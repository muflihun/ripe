//
//  Ripe.h
//
//  Copyright © 2017 Muflihun Labs. All rights reserved.
//
//  http://muflihun.com
//  https://muflihun.github.io/ripe
//  https://github.com/muflihun
//

#ifndef Ripe_h
#define Ripe_h

#include <string>
#include <vector>

using byte = unsigned char;

#ifdef RIPE_DLL
#ifdef RIPE_EXPORTS
#define RIPE_API __declspec(dllexport)
#else
#define RIPE_API __declspec(dllimport)
#endif
#else
#define RIPE_API
#endif

///
/// \brief The Ripe class is core of Ripe library and contains all
/// the required cryptography API supported by Ripe
///
RIPE_API class Ripe {
public:

    ///
    /// \brief Data delimiter for prepared data
    /// \see prepareData(const char*, const std::string&, const char*)
    ///
    static const char DATA_DELIMITER;

    ///
    /// \brief Constant value for bits per bytes (8)
    ///
    static const int BITS_PER_BYTE;

    ///
    /// \brief Constant value default rsa length
    ///
    static const int DEFAULT_RSA_LENGTH;

    ///
    /// \brief Constant value for AES block size
    ///
    static const int AES_BSIZE;

    ///
    /// \brief RSA Key pair
    ///
    struct KeyPair {
        std::string privateKey;
        std::string publicKey;
    };

    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             AES                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                             CRYPTO                              *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/


    ///
    /// \brief Encrypts data of length with symmetric key of size = keySize with specified initialization vector
    ///
    static std::string encryptAES(const char* data, const byte* key, std::size_t keySize, std::vector<byte>& iv);

    ///
    /// \brief Decrypts data of specified length with specified key and initialization vector
    ///
    static std::string decryptAES(const std::string& data, const byte* key, std::size_t keySize, std::vector<byte>& iv);

    ///
    /// \brief Generate random AES key
    /// \param length Length of key, must be 16, 24 or 32
    /// \return Hexadecimal value of key
    ///
    static std::string generateNewKey(int length);




    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             AES                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                            HELPERS                              *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/


    ///
    /// \brief encryptAES Encrypts data with provided symmetric key
    /// \param outputFile Optional, if provided instead of printing it to console data is saved to file and IV is printed on console
    ///
    static std::string encryptAES(std::string& data, const std::string& hexKey, const std::string& clientId, const std::string& outputFile);

    ///
    /// \brief Helper function that takes hex key
    /// \see encryptAES(std::string& data, const std::string& hexKey, const std::string& clientId, const std::string& outputFile)
    ///
    static inline std::string encryptAES(const std::string& buffer, const std::string& hexKey, std::vector<byte>& iv)
    {
        return encryptAES(buffer.c_str(), reinterpret_cast<const byte*>(hexToString(hexKey).c_str()), hexKey.size() / 2, iv);
    }

    ///
    /// \brief decryptAES Decrypts data using specified symmetric key.
    /// \param isBase64 If true, first base64 decoding is done on data and then decryption is processed
    /// \param isHex If true, hex string is decoded. If base64 is true, base64 decoding is done before hex decoding
    ///
    static std::string decryptAES(std::string& data, const std::string& hexKey, std::string& iv, bool isBase64 = false, bool isHex = false);


    ///
    /// \brief Exceptect size of AES cipher when plainDataSize size data is encrypted
    ///
    static inline std::size_t expectedAESCipherLength(std::size_t plainDataSize) noexcept
    {
        return (plainDataSize / AES_BSIZE + 1) * AES_BSIZE;
    }

    ///
    /// \brief normalizeIV If IV with no space is provided e.g, <pre>67e56fee50e22a8c2ba05c0fb2932bfa:</pre> normalized IV
    /// is <pre>67 e5 6f ee 50 e2 2a 8c 2b a0 5c 0f b2 93 2b fa:</pre>
    ///
    static bool normalizeHex(std::string& iv) noexcept;



    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             RSA                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                            CRYPTO                               *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/


    ///
    /// \brief Encrypts data of length = dataLength using RSA key and puts it in destination
    /// \return The size of the encrypted data. On error -1 is returned. use printLastError(const char*) to see the error details
    ///
    static std::string encryptRSA(const std::string& data, const std::string& publicKeyPEM);

    ///
    /// \brief Decrypts encryptedData of length dataLength with RSA key and puts result in destination
    /// \return The size of the recovered plaintext. On error -1 is returned. use printLastError(const char* name) to see the error details
    ///
    static std::string decryptRSA(const std::string& data, const std::string& privateKeyPEM, const std::string& secret = "");

    ///
    /// \brief Generate key pair and returns KeyPair
    /// \see KeyPair
    ///
    static KeyPair generateRSAKeyPair(unsigned int length = DEFAULT_RSA_LENGTH);




    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             RSA                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                           HELPERS                               *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/

    ///
    /// \brief maxRSABlockSize Maximum size of RSA block with specified key size
    ///
    ///
    static inline unsigned int maxRSABlockSize(std::size_t keySize)
    {
        return ((keySize - 384) / 8) + 7;
    }

    ///
    /// \brief encryptRSA Encrypts using RSA key
    /// \param outputFile Optional, if provided instead of printing it to console data is saved to file
    /// \param isRaw Outputs raw data
    ///
    static std::string encryptRSA(std::string& data, const std::string& key, const std::string& outputFile, bool isRaw = false);

    ///
    /// \brief decryptRSA Decrypts using RSA key
    /// \param isHex If true, hex string is decoded. If base64 is true, base64 decoding is done before hex decoding
    /// \param isBase64 If true, first base64 decoding is done on data and then decryption is processed
    ///
    static std::string decryptRSA(std::string& data, const std::string& key, bool isBase64, bool isHex = false, const std::string& secret = "");

    ///
    /// \brief writeRSAKeyPair Writes RSA key pair to public and private file paths.
    /// \param length RSA key size
    ///
    static bool writeRSAKeyPair(const std::string& publicFile, const std::string& privateFile, int length = DEFAULT_RSA_LENGTH);

    ///
    /// \brief generateRSAKeyPair Generates RSA key pair and returns colon seperated base64 where first part is private key and second part is public key.
    ///
    static std::string generateRSAKeyPairBase64(int length = DEFAULT_RSA_LENGTH);



    /*****************************************************************************************************/

                /*******************************************************************\
                 *                            MISC                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                           Base64                                *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/

    ///
    /// \brief Decodes encoded base64
    ///
    static std::string base64Decode(const std::string& base64Encoded);

    ///
    /// \brief Encodes input of length to base64 encoding
    ///
    static std::string base64Encode(const std::string& binaryData);

    ///
    /// \brief expectedBase64Length Returns expected base64 length
    /// \param n Length of input (plain data)
    ///
    static inline std::size_t expectedBase64Length(std::size_t n) noexcept
    {
        return ((4 * n / 3) + 3) & ~0x03;
    }



    /*****************************************************************************************************/

                /*******************************************************************\
                 *                            MISC                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                           OTHERS                                *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/

    ///
    /// \brief prepareData Helper method to encrypt data with symmetric key and convert it in to tranferable data.
    /// \param clientId Extra text in between representing client ID (leave empty if you don't need it)
    /// \return Base64 format of encrypted data with format: <pre>[LENGTH]:[IV]:[<Client_ID>:]:[Base64 Data]</pre>
    ///
    static std::string prepareData(const char* data, const std::string& hexKey, const char* clientId = "");

    ///
    /// \brief Calculates expected data size. Assumed IV size = 32
    /// \see prepareData(const char*, const std::string&, const char*)
    ///
    static std::size_t expectedDataSize(std::size_t plainDataSize, std::size_t clientIdSize = 16) noexcept;

    ///
    /// \brief Helper function to convert string to hexdecimal e.g, khn = 6b686e.
    ///
    static std::string stringToHex(const std::string& raw) noexcept;

    ///
    /// \brief Helper function to convert hexadecimal input to raw data
    ///
    static std::string hexToString(const std::string& hex);

    ///
    /// \brief Converts vector of byte to raw string
    ///
    static std::string vecToString(const std::vector<byte>& iv) noexcept;

    ///
    /// \brief ivToVector Converts plain (unsigned char*) IV to std::vector<byte>
    ///
    static std::vector<byte> byteToVec(const byte* iv) noexcept;

    ///
    /// \brief version Version of Ripe library
    ///
    static std::string version() noexcept;
};
#endif /* Ripe_h */
