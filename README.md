# cryptoauth_usecase_ipprotection_symmetric

The Microchip CryptoAuthentication™ devices are a family of high-security secure elements, which combine world-class hardware-based key storage with hardware cryptographic accelerators to implement various authentication and encryption protocols.

CryptoAuthLib is a software-support library for CryptoAuthentication devices, written in C. It is a portable, extensible, powerful and easy-to-use library for working with the ATSHA and ATECC family devices.

This use case example user guide talks about how Intellectual Property (IP) protection can be implemented using Microchip Crypto Authentication devices in a simple and secure way. This demonstrates protecting application firmware with symmetric authentication. The demo code is based on an ASF example and CryptoAuthLib, which can be easily ported to other platforms.

# Hardware and Software Requirements
In order to perform the IP protection demonstration, the following hardware and software requirements must be met.

## Hardware Prerequisites:
 - SAMD21 Xplained Pro Evaluation Board
 - Any one of the below boards connected to EXT2
   1. CryptoAuth-XPRO socket kits (AT88CKSCKTUDFN-XPRO or AT88CKSCKTSOIC-XPRO) with CryptoAuthentication device.
   2. ATCRYPTOAUTH-XPRO-B board.
 - OLED1 Xplained Pro, to be connected on EXT3
 - One Micro-B to Type-A USB interface cable

## Software Prerequisites:
 - Atmel Studio 7
 - Atmel Software Framework (ASF) 3.34 or later
 - CryptoAuthLib
 - Serial Console like Tera Term/Putty (Optional)
**Note:** All pieces of software listed above can be downloaded from the Microchip website.

# References
 - [Security ICs Overview](http://www.microchip.com/design-centers/security-ics/overview)
 - [CryptoAuthLib](http://www.microchip.com/SWLibraryWeb/product.aspx?product=CryptoAuthLib)
 - [Atmel Studio 7](http://www.microchip.com/avr-support/atmel-studio-7)
