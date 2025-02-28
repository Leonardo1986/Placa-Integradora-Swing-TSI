#ifndef VERSION_H_
#define VERSION_H_

#define SELFCHECKOUT 1
#define EVO			 2
#define SLIM         3
#define CLASSIC      4

#define FIRMWARE_VERSION_MAJOR CLASSIC
#define FIRMWARE_VERSION_MINOR 1
#define FIRMWARE_VERSION_PATCH 10

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define FIRMWARE_VERSION_STRING STR(FIRMWARE_VERSION_MAJOR) "." \
                                STR(FIRMWARE_VERSION_MINOR) "." \
                                STR(FIRMWARE_VERSION_PATCH)

#define WARNING_MESSAGE "Versão do Firmware: " FIRMWARE_VERSION_STRING

#pragma message(WARNING_MESSAGE)
// Console '#pragma message: VersÃ£o do Firmware: 1.1.10'

#endif /* VERSION_H_ */
