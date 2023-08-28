#ifndef BH1726_DEFS_H_
#define BH1726_DEFS_H_

#include <stdint.h>
#include <stdbool.h>

#define BH1726_PRODUCT_ID               0x72

/** @brief 				I2C write function prototype.
 *  @param DeviceAddr	Device address
 *  @param p_Buffer		Pointer to data buffer
 *  @param Length		Buffer length
 *  @return				0 when successful
 * 						-1 when not successful
 */
typedef int32_t (*I2C_Write_t)(uint8_t DeviceAddr, const uint8_t* p_Buffer, uint32_t Length);

/** @brief 				I2C read function prototype.
 *  @param DeviceAddr	Device address
 *  @param p_Buffer		Pointer to data buffer
 *  @param Length		Buffer length
 *  @return				0 when successful
 * 						-1 when not successful
 */
typedef int32_t (*I2C_Read_t)(uint8_t DeviceAddr, uint8_t* p_Buffer, uint32_t Length);

/** @brief          Milliseconds delay function prototype.
 *  @param Delay	Delay time
 */
typedef void (*Delay_ms_t)(uint32_t Delay);

/** @brief BH1726 gain options.
 */
typedef enum
{
    BH1726_GAIN_1 = 0,                      /**< x1 gain mode. */
    BH1726_GAIN_2,                          /**< x2 gain mode. */
    BH1726_GAIN_64,                         /**< x64 gain mode. */
    BH1726_GAIN_128,                        /**< x128 gain mode. */
} BH1726_Gain_t;

/** @brief BH1726 device object definition.
 */
typedef struct
{
    I2C_Read_t I2C_Read;                    /**< Function pointer for I2C read function. */
    I2C_Write_t I2C_Write;                  /**< Function pointer for I2C read function. */
    Delay_ms_t Delay;                       /**< Function pointer for a milliseconds delay function. */
    uint8_t DevAddr;                        /**< Device address. */
    uint8_t ProductID;                      /**< Product ID from the initialization process. */
    bool isPowerDown;                       /**< #true when the device is in power-down mode. */
    bool isInterruptActive;                 /**< #true when the device interrupts are enabled. */
    bool isInitialized;                     /**< #true when the device is initialized. */
} BH1726_t;

#endif /* BH1726_DEFS_H_ */