

package main

import (
		"bytes"
	"crypto/aes"
	"crypto/ed25519"
	"crypto/cipher"
	"crypto/sha256"
	"encoding/base64"
	"fmt"
	"math/rand"
)

// CBCEncrypt . aes_cbc_256_enc, matching with pitaya sdk aes_enc, （plain, key) --> (iv||ciphertext)
// len(plaintext) > 0, len(key) must be 16/24/32
func CBCEncrypt(plaintext string, key string) string {
	defer func() {
		if err := recover(); err != nil {
			fmt.Println("cbc enc err:", err)
		}
	}()

	block, err := aes.NewCipher([]byte(key))
	if err != nil {
		return ""
	}

	// pkcs7 padding, openssl default mode
	// blockSize := len(key)
    blockSize := block.BlockSize()
	padding := blockSize - len(plaintext)%blockSize
	if padding == 0 {
		padding = blockSize
	}

	plaintext += string(bytes.Repeat([]byte{byte(padding)}, padding))
	ciphertext := make([]byte, aes.BlockSize+len(plaintext))
	iv := ciphertext[:aes.BlockSize]
	if _, err = rand.Read(iv); err != nil {
		return ""
	}

        for i:=0; i < aes.BlockSize; i++ {
        	iv[i] = 0x0;
        }
	mode := cipher.NewCBCEncrypter(block, iv)
	mode.CryptBlocks(ciphertext[aes.BlockSize:], []byte(plaintext))

//	return base64.StdEncoding.EncodeToString(ciphertext)
        return string(ciphertext)
}

// CBCDecrypt AES-CBC 解密
func CBCDecrypt(ciphertext string, key string) string {
    defer func() {
        if err := recover(); err != nil {
            fmt.Println("cbc decrypt err:", err)
        }
    }()

    block, err := aes.NewCipher([]byte(key))
    if err != nil {
        return ""
    }

    ciphercode, err := base64.StdEncoding.DecodeString(ciphertext)
    if err != nil {
        return ""
    }

    iv := ciphercode[:aes.BlockSize]        // 密文的前 16 个字节为 iv
    ciphercode = ciphercode[aes.BlockSize:] // 正式密文

    mode := cipher.NewCBCDecrypter(block, iv)
    mode.CryptBlocks(ciphercode, ciphercode)

    plaintext := string(ciphercode) // ↓ 减去 padding
    return plaintext[:len(plaintext)-int(plaintext[len(plaintext)-1])]
}


func main() {

   did := "123123"
   key := sha256.Sum256([]byte(did))
   
   _, priKey, err := ed25519.GenerateKey(nil)
   if err != nil {
   }

   fmt.Println("priKey = ", priKey)
   
   cipher := CBCEncrypt(string(priKey[:32]), string(key[:]))
   fmt.Println("cipher = ", []byte(cipher))
   for i := 0; i < len(cipher); i++ {
	fmt.Printf("0x%02x,", []byte(cipher)[i])
   }
}
