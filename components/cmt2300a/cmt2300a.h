/**
 * @file cmt2300a.h
 * @brief CMT2300A Radio Hamming(7,4) FEC Codec
 *
 * This class provides Hamming(7,4) forward error correction encoding and decoding
 * for the CMT2300A radio transceiver.
 *
 * Generator polynomial: g(x) = x³ + x² + 1 (0xD)
 *
 * Code Structure:
 *   [d3 d2 d1 d0 | p2 p1 p0]
 *   - Bits [6:3]: Data bits (4 bits)
 *   - Bits [2:0]: Parity bits (3 bits)
 *
 * Parity Equations:
 *   p2 = d3 ⊕ d1 ⊕ d0
 *   p1 = d3 ⊕ d2 ⊕ d1
 *   p0 = d2 ⊕ d1 ⊕ d0
 */

#ifndef CMT2300A_H
#define CMT2300A_H

#include "esp_log.h"
#include <cstdint>
#include <vector>
#include <limits>

class CMT2300A {
 public:
  /**
   * @brief Encode a single 4-bit nibble into a 7-bit Hamming codeword
   *
   * @param nibble 4-bit data value (0x0-0xF)
   * @return 7-bit Hamming codeword
   */
  static inline uint8_t encode_nibble(uint8_t nibble) {
    // Mask to 4 bits
    nibble &= 0x0F;

    // Compute parity bits
    uint8_t parity = compute_parity(nibble);

    // Create systematic codeword: [d3 d2 d1 d0 | p2 p1 p0]
    uint8_t codeword = (nibble << 3) | parity;

    return codeword;
  }

  /**
   * @brief Encode a stream of bytes with FEC protection
   *
   * Each byte is split into two nibbles, each encoded as a 7-bit codeword,
   * and packed into a continuous 14-bit block stream. The size of the data
   * is prepended as the first encoded byte to allow automatic size detection
   * during decoding.
   *
   * @param data Input data bytes to encode (max 255 bytes)
   * @param encoded Output vector to receive FEC-protected bytes (~1.75x input size + size byte)
   */
  static inline void encode(const std::vector<uint8_t> &data, std::vector<uint8_t> &encoded) {
    uint32_t accumulator = (encode_nibble(data.size() >> 4) << 7) | encode_nibble(data.size() & 0x0F);
    int bits_in_accumulator = 14;

    encoded.clear();

    for (uint8_t byte : data) {
      uint8_t high_codeword = encode_nibble(byte >> 4);
      uint8_t low_codeword = encode_nibble(byte & 0x0F);

      // Combine into 14-bit block: [high_codeword (7 bits)][low_codeword (7 bits)]
      uint16_t block_14 = (static_cast<uint16_t>(high_codeword) << 7) | low_codeword;

      // Add to accumulator
      accumulator = (accumulator << 14) | block_14;
      bits_in_accumulator += 14;

      // Extract complete bytes from accumulator
      while (bits_in_accumulator >= 8) {
        // Extract top 8 bits
        uint8_t output_byte = (accumulator >> (bits_in_accumulator - 8)) & 0xFF;
        encoded.push_back(output_byte);
        bits_in_accumulator -= 8;
      }
    }

    // Flush remaining bits (if any)
    if (bits_in_accumulator > 0) {
      // Shift remaining bits to the left and pad with zeros
      uint8_t output_byte = (accumulator << (8 - bits_in_accumulator)) & 0xFF;
      encoded.push_back(output_byte);
    }
  }

  /**
   * @brief Decode and error-correct a single 7-bit codeword
   *
   * Detects and corrects single-bit errors, extracts the 4-bit data nibble.
   *
   * @param codeword 7-bit Hamming codeword
   * @param error_corrected Output reference - set to true if error was corrected, false otherwise
   * @return 4-bit decoded nibble (0x0-0xF)
   */
  static inline uint8_t decode_nibble(uint8_t codeword, bool &error_corrected) {
    // Extract data and parity bits from the systematic code [d3 d2 d1 d0 | p2 p1 p0]
    uint8_t d3 = (codeword >> 6) & 1;
    uint8_t d2 = (codeword >> 5) & 1;
    uint8_t d1 = (codeword >> 4) & 1;
    uint8_t d0 = (codeword >> 3) & 1;

    uint8_t p2_received = (codeword >> 2) & 1;
    uint8_t p1_received = (codeword >> 1) & 1;
    uint8_t p0_received = (codeword >> 0) & 1;

    // Recompute parity bits from received data
    uint8_t p2_computed = d3 ^ d1 ^ d0;
    uint8_t p1_computed = d3 ^ d2 ^ d1;
    uint8_t p0_computed = d2 ^ d1 ^ d0;

    // Calculate syndrome (difference between computed and received parity)
    uint8_t s2 = p2_computed ^ p2_received;
    uint8_t s1 = p1_computed ^ p1_received;
    uint8_t s0 = p0_computed ^ p0_received;

    uint8_t syndrome = (s2 << 2) | (s1 << 1) | s0;

    // No error detected
    if (syndrome == 0) {
      error_corrected = false;
      return (d3 << 3) | (d2 << 2) | (d1 << 1) | d0;
    }

    // Single-bit error detected - correct it
    error_corrected = true;

    // Apply correction by flipping the erroneous bit
    uint8_t corrected_codeword = codeword;

    switch (syndrome) {
      case 0b001: // Error in p0 (bit 0)
        corrected_codeword ^= (1 << 0);
        break;
      case 0b010: // Error in p1 (bit 1)
        corrected_codeword ^= (1 << 1);
        break;
      case 0b011: // Error in d2 (bit 5)
        corrected_codeword ^= (1 << 5);
        break;
      case 0b100: // Error in p2 (bit 2)
        corrected_codeword ^= (1 << 2);
        break;
      case 0b101: // Error in d0 (bit 3)
        corrected_codeword ^= (1 << 3);
        break;
      case 0b110: // Error in d3 (bit 6)
        corrected_codeword ^= (1 << 6);
        break;
      case 0b111: // Error in d1 (bit 4)
        corrected_codeword ^= (1 << 4);
        break;
      default:
        // Should not happen for valid single-bit errors
        break;
    }

    // Extract corrected data bits
    d3 = (corrected_codeword >> 6) & 1;
    d2 = (corrected_codeword >> 5) & 1;
    d1 = (corrected_codeword >> 4) & 1;
    d0 = (corrected_codeword >> 3) & 1;

    return (d3 << 3) | (d2 << 2) | (d1 << 1) | d0;
  }

  /**
   * @brief Decode a stream of FEC-encoded bytes with error correction
   *
   * Processes a continuous stream of 14-bit blocks, decoding and correcting
   * errors automatically. The first decoded byte contains the expected size
   * of the output data, allowing the decoder to stop at the correct length.
   *
   * @param encoded Input encoded byte stream (must include size byte)
   * @param decoded Output vector to receive decoded bytes (excluding the size byte)
   * @return Number of single-bit errors corrected during decoding
   */
  static inline uint32_t decode(const std::vector<uint8_t> &encoded, std::vector<uint8_t> &decoded) {
    std::size_t size = std::numeric_limits<std::size_t>::max();
    uint32_t remainder = 0;
    uint32_t data = 0;

    uint32_t total_codewords = 0;
    uint32_t corrected_errors = 0;

    decoded.clear();

    // Process the input byte stream
    for (uint8_t byte : encoded) {
      // Shift in new byte
      data = (data << 8) | byte;
      remainder += 8;

      // Extract and decode 14-bit blocks
      if (remainder >= 14) {
        // Extract 14-bit block from the bit stream
        uint16_t block_14 = (data >> (remainder - 14)) & 0x3FFF;

        // Split into two 7-bit codewords
        // High 7 bits [13:7] encode the high nibble
        // Low 7 bits [6:0] encode the low nibble
        uint8_t high_codeword = (block_14 >> 7) & 0x7F;
        uint8_t low_codeword = block_14 & 0x7F;

        // Decode both codewords with error correction
        bool high_corrected = false;
        bool low_corrected = false;

        uint8_t high_nibble = decode_nibble(high_codeword, high_corrected);
        uint8_t low_nibble = decode_nibble(low_codeword, low_corrected);

        if (high_corrected || low_corrected) {
          ESP_LOGV("CMT2300A", "Error in byte %d", decoded.size());
        }

        // Update statistics
        total_codewords += 2;
        if (high_corrected) corrected_errors++;
        if (low_corrected) corrected_errors++;

        // Combine nibbles into output byte
        uint8_t decoded_byte = (high_nibble << 4) | low_nibble;
        if (size == std::numeric_limits<std::size_t>::max()) {
          size = decoded_byte;
        } else {
          decoded.push_back(decoded_byte);
          if (decoded.size() >= size) {
            break;
          }
        }
        remainder -= 14;
      }
    }
    return corrected_errors;
  }

 private:
  /**
   * @brief Compute 3-bit parity for a 4-bit data nibble
   *
   * @param nibble 4-bit data nibble
   * @return 3-bit parity value
   */
  static inline uint8_t compute_parity(uint8_t nibble) {
    // Extract data bits
    uint8_t d3 = (nibble >> 3) & 1;
    uint8_t d2 = (nibble >> 2) & 1;
    uint8_t d1 = (nibble >> 1) & 1;
    uint8_t d0 = nibble & 1;

    // Compute parity bits according to generator polynomial g(x) = x³ + x² + 1
    // p2 = d3 ⊕ d1 ⊕ d0
    // p1 = d3 ⊕ d2 ⊕ d1
    // p0 = d2 ⊕ d1 ⊕ d0
    uint8_t p2 = d3 ^ d1 ^ d0;
    uint8_t p1 = d3 ^ d2 ^ d1;
    uint8_t p0 = d2 ^ d1 ^ d0;

    return (p2 << 2) | (p1 << 1) | p0;
  }
};

#endif // CMT2300A_H
