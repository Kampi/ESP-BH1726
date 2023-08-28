#ifndef BH1726_ERRORS_H_
#define BH1726_ERRORS_H_

#include <esp_log.h>

#include <sdkconfig.h>

#ifndef CONFIG_BH1726_MISC_ERROR_BASE
    #define CONFIG_BH1726_MISC_ERROR_BASE                       0x7000
#endif

typedef int BH1726_Error_t;

/** @brief      Generic error check macro.
 *  @param Func Function that should be checked
 */
#define BH1726_ERROR_CHECK(Func)                                do                                                                                                              \
                                                                {                                                                                                               \
                                                                    BH1726_Error_t Error = Func;                                                                              \
                                                                    if(Error != BH1726_ERR_OK)                                                                                \
                                                                    {                                                                                                           \
                                                                        ESP_LOGE("BH1726", "Error check failed in (%s) at line (%u): 0x%X!", __FUNCTION__, __LINE__, Error);  \
                                                                        return Error;                                                                                           \
                                                                    }                                                                                                           \
                                                                } while(0);

/** @brief BH1726 error base.
 */
#define BH1726_BASE                                             CONFIG_BH1726_MISC_ERROR_BASE

/** @brief Everything okay.
 */
#define BH1726_ERR_OK                                           (BH1726_BASE + 0)

/** @brief Invalid function parameter.
 */
#define BH1726_ERR_INVALID_ARG                                  (BH1726_BASE + 1)

/** @brief Invalid response from sensor.
 */
#define BH1726_ERR_INVALID_RESPONSE                             (BH1726_BASE + 2)

/** @brief No device found.
 */
#define BH1726_ERR_NO_DEVICE                                    (BH1726_BASE + 3)

/** @brief Device not initialized.
 */
#define BH1726_ERR_NOT_INITIALIZED                              (BH1726_BASE + 2)

#endif /* BH1726_ERRORS_H_ */