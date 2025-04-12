# .bwc file

## File ID
 - Byte: 0-3 | Offset 0
 - BWCF in ascii: 0x42574346 (Big Endian)
 - Symbolizes the file id

## Version Tag (Current: 1)
 - Byte: 4-7 | Offset 4
 - 32 Bit Unsigned Integer, represents the version number (Little Endian)

## Host Api
 - Byte: 8 - 11 | Offset 8
 - 4 Letter Host API Tag spelled out in ascii (Big Endian)
 - Host API tags defined in <bw-config.h>

## Device Name Length
 - Byte: 12 - 15 | Offset 12
 - 32 Bit Unsigned Integer, number of bytes in the next section (Little Endian)

## Device Name
 - Byte: 16 - 16 + (Name Length - 1) | Offset 16
 - Device Name, ascii (Big Endian)

## Screen Width and Height
 - Byte: N - N + 7 | Offset N
 - First 4 Bytes: Width of the Screen
 - Second 4 Bytes: Height of the Screen
