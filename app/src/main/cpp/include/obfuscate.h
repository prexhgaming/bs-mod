#pragma once

/* ===========================================================================
 * Consolidated string obfuscation header (single source of truth).
 *
 * Provides two macro families:
 *   O("literal")        -> _o53::_3z<...>&  (.str() / .chr(), manual re-lock)
 *   OBFUSCATE("literal")-> ay::obfuscated_data<...>& (implicit char* / std::string)
 *
 * The OBFUSCATE part is Adam Yaxley's obfuscate.h (public domain / Unlicense),
 * merged in here so the project keeps a single obfuscation header instead of a
 * second copy at the jni root.
 * ===========================================================================*/

#include <cstddef>
#include <string>

/* ---------------------------------------------------------------------------
 * Part 1: original _o53 / O(...) obfuscation
 * -------------------------------------------------------------------------*/
#define _dgo27 _o53::h525_()

namespace _o53 {
    using da7 = unsigned long long;
    using b96 = unsigned long long;

    constexpr b96 h525_(b96 seed = 0) {
        b96 gd926 = static_cast< int >(__TIME__[7]) + static_cast< int >(__TIME__[6]) * 10 +
                    static_cast< int >(__TIME__[4]) * 60 + static_cast< int >(__TIME__[3]) * 600 +
                    static_cast< int >(__TIME__[1]) * 3600 + static_cast< int >(__TIME__[0]) * 36000;
        if (seed) gd926 += seed;
        gd926 ^= (gd926 >> 33);
        gd926 *= 0xff51afd7ed558ccd;
        gd926 ^= (gd926 >> 33);
        gd926 *= 0xc4ceb9fe1a85ec53;
        gd926 ^= (gd926 >> 33);
        gd926 |= 0x0101010101010101ull;
        return gd926;
    }

    constexpr void ew85(char *e0r_, da7 tag752, b96 gd926) {
        for (da7 i = 0; i < tag752; i++) {
            e0r_[i] ^= char(gd926 >> ((i % 8) * 8));
        }
    }

    template<da7 N, b96 ky9>
    class b9_ {
    public:
        constexpr explicit b9_(const char *e0r_) {
            for (da7 i = 0; i < N; i++) {
                h7982[i] = e0r_[i];
            }
            ew85(h7982, N, ky9);
        }

        [[nodiscard]] constexpr const char *e0r_() const {
            return &h7982[0];
        }

        [[maybe_unused]] [[nodiscard]] constexpr da7 tag752() const {
            return N;
        }

        [[maybe_unused]] [[nodiscard]] constexpr b96 gd926() const {
            return ky9;
        }

    private:
        char h7982[N]{};
    };

    template<da7 N, b96 ky9>
    class _3z {
    public:
        explicit _3z(const b9_<N, ky9> &b9_) {
            for (da7 i = 0; i < N; i++) {
                h7982[i] = b9_.e0r_()[i];
            }
        }

        ~_3z() {
            for (da7 i = 0; i < N; i++) {
                h7982[i] = 0;
            }
        }

        /* Implicit conversion so O("...") can be used wherever a string is
         * expected, exactly like OBFUSCATE() from the section below. */
        const char *c_str() {
            wync9();
            return h7982;
        }

        operator const char *() {
            return c_str();
        }

        explicit operator std::string() {
            wync9();
            return h7982;
        }

        char *chr() {
            wync9();
            return h7982;
        }

        std::string str() {
            wync9();
            return h7982;
        }

        void wync9() {
            if (_yr9) {
                ew85(h7982, N, ky9);
                _yr9 = false;
            }
        }

        [[maybe_unused]] void jfp1o() {
            if (!_yr9) {
                ew85(h7982, N, ky9);
                _yr9 = true;
            }
        }

        [[maybe_unused]] [[nodiscard]] bool h7a9mys() const {
            return _yr9;
        }

    private:
        char h7982[N]{};
        bool _yr9{true};
    };

    template<da7 N, b96 ky9 = _dgo27>
    constexpr auto j62_(const char(&e0r_)[N]) {
        return b9_<N, ky9>(e0r_);
    }
}

#define O(e0r_) []() -> _o53::_3z<sizeof(e0r_)/sizeof(e0r_[0]), _dgo27>& { constexpr auto n = sizeof(e0r_)/sizeof(e0r_[0]); constexpr auto b9_ = _o53::j62_<n, _dgo27>(e0r_); static auto _3z = _o53::_3z<n, _dgo27>(b9_); return _3z; }()

/* ---------------------------------------------------------------------------
 * Part 2: Adam Yaxley obfuscate.h — OBFUSCATE() / OBFUSCATE_KEY()
 *
 * Original Author: Adam Yaxley
 * Website: https://github.com/adamyaxley
 * License: public domain (Unlicense) — see the notice at the end of this file.
 *
 * Pass string literals into OBFUSCATE() to obfuscate them at compile time.
 * It returns a reference to an ay::obfuscated_data with global lifetime that is
 * implicitly convertible to char* and std::string.
 * -------------------------------------------------------------------------*/

#ifndef AY_OBFUSCATE_DEFAULT_KEY
// The default 64 bit key to obfuscate strings with.
// This can be user specified by defining AY_OBFUSCATE_DEFAULT_KEY before
// including obfuscate.h
#define AY_OBFUSCATE_DEFAULT_KEY ay::generate_key(__LINE__)
#endif

namespace ay
{
    using size_type = unsigned long long;
    using key_type = unsigned long long;

    // Generate a psuedo-random key that spans all 8 bytes
    constexpr key_type generate_key(key_type seed)
    {
        // Use the MurmurHash3 64-bit finalizer to hash our seed
        key_type key = seed;
        key ^= (key >> 33);
        key *= 0xff51afd7ed558ccd;
        key ^= (key >> 33);
        key *= 0xc4ceb9fe1a85ec53;
        key ^= (key >> 33);

        // Make sure that a bit in each byte is set
        key |= 0x0101010101010101ull;

        return key;
    }

    // Obfuscates or deobfuscates data with key
    constexpr void cipher(char* data, size_type size, key_type key)
    {
        // Obfuscate with a simple XOR cipher based on key
        for (size_type i = 0; i < size; i++)
        {
            data[i] ^= char(key >> ((i % 8) * 8));
        }
    }

    // Obfuscates a string at compile time
    template <size_type N, key_type KEY>
    class obfuscator
    {
    public:
        // Obfuscates the string 'data' on construction
        constexpr obfuscator(const char* data)
        {
            // Copy data
            for (size_type i = 0; i < N; i++)
            {
                m_data[i] = data[i];
            }

            // On construction each of the characters in the string is
            // obfuscated with an XOR cipher based on key
            cipher(m_data, N, KEY);
        }

        constexpr const char* data() const
        {
            return &m_data[0];
        }

        constexpr size_type size() const
        {
            return N;
        }

        constexpr key_type key() const
        {
            return KEY;
        }

    private:

        char m_data[N]{};
    };

    // Handles decryption and re-encryption of an encrypted string at runtime
    template <size_type N, key_type KEY>
    class obfuscated_data
    {
    public:
        obfuscated_data(const obfuscator<N, KEY>& obfuscator)
        {
            // Copy obfuscated data
            for (size_type i = 0; i < N; i++)
            {
                m_data[i] = obfuscator.data()[i];
            }
        }

        ~obfuscated_data()
        {
            // Zero m_data to remove it from memory
            for (size_type i = 0; i < N; i++)
            {
                m_data[i] = 0;
            }
        }

        // Returns a pointer to the plain text string, decrypting it if
        // necessary
        operator char*()
        {
            decrypt();
            return m_data;
        }

        operator std::string()
        {
            decrypt();
            return m_data;
        }

        // Manually decrypt the string
        void decrypt()
        {
            if (m_encrypted)
            {
                cipher(m_data, N, KEY);
                m_encrypted = false;
            }
        }

        // Manually re-encrypt the string
        void encrypt()
        {
            if (!m_encrypted)
            {
                cipher(m_data, N, KEY);
                m_encrypted = true;
            }
        }

        // Returns true if this string is currently encrypted, false otherwise.
        bool is_encrypted() const
        {
            return m_encrypted;
        }

    private:

        // Local storage for the string. Call is_encrypted() to check whether or
        // not the string is currently obfuscated.
        char m_data[N];

        // Whether data is currently encrypted
        bool m_encrypted{ true };
    };

    // This function exists purely to extract the number of elements 'N' in the
    // array 'data'
    template <size_type N, key_type KEY = AY_OBFUSCATE_DEFAULT_KEY>
    constexpr auto make_obfuscator(const char(&data)[N])
    {
        return obfuscator<N, KEY>(data);
    }
}

// Obfuscates the string 'data' at compile-time and returns a reference to a
// ay::obfuscated_data object with global lifetime that has functions for
// decrypting the string and is also implicitly convertable to a char*
#define OBFUSCATE(data) OBFUSCATE_KEY(data, AY_OBFUSCATE_DEFAULT_KEY)

// Obfuscates the string 'data' with 'key' at compile-time and returns a
// reference to a ay::obfuscated_data object with global lifetime that has
// functions for decrypting the string and is also implicitly convertable to a
// char*
#define OBFUSCATE_KEY(data, key) \
	[]() -> ay::obfuscated_data<sizeof(data)/sizeof(data[0]), key>& { \
		static_assert(sizeof(decltype(key)) == sizeof(ay::key_type), "key must be a 64 bit unsigned integer"); \
		static_assert((key) >= (1ull << 56), "key must span all 8 bytes"); \
		constexpr auto n = sizeof(data)/sizeof(data[0]); \
		constexpr auto obfuscator = ay::make_obfuscator<n, key>(data); \
		static auto obfuscated_data = ay::obfuscated_data<n, key>(obfuscator); \
		return obfuscated_data; \
	}()

/* -------------------------------- LICENSE ------------------------------------
Public Domain (http://www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------- */
