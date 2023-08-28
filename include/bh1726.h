#ifndef BH1726_H_
#define BH1726_H_

#include <esp_err.h>

#include "bh1726_defs.h"
#include "bh1726_errors.h"

/** @brief          Initialize the driver.
 *  @param p_Device BH1726 device object
 *  @return         BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_Init(BH1726_t& p_Device);

/** @brief          Deinitialize the driver.
 *  @param p_Device BH1726 device object
 *  @return         BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_Deinit(BH1726_t& p_Device);

/** @brief          Enable / Disable the power-down mode of the sensor.
 *  @param p_Device BH1726 device object
 *  @param Enable   Enable / Disable
 *  @return         BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_PowerDown(BH1726_t& p_Device, bool Enable);

/** @brief          Set the sensor gain
 *  @param p_Device BH1726 device object
 *  @param Gain0    Sensor gain for diode 0
 *  @param Gain1    Sensor gain for diode 1
 *  @return         BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_SetIntegrationTime(BH1726_t& p_Device, BH1726_Gain_t Gain0, BH1726_Gain_t Gain1);

/** @brief          Set the integration time for a measurement cycle.
 *  @param p_Device BH1726 device object
 *  @param Time     Integration time
 *                  NOTE: See the datasheet for the calculation formular
 *  @return         BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_SetIntegrationTime(BH1726_t& p_Device, uint8_t Time);

/** @brief          Perform a measurement.
 *  @param p_Device BH1726 device object
 *  @param p_Result Pointer to measurement result
 *  @param UseD1    (Optional) Use D1 instead of D0 for the measurement
 *  @return         BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_Measure(BH1726_t& p_Device, uint16_t* p_Result, bool UseD1 = false);

/** @brief                  Enable the sensor interrupts.
 *  @param p_Device         BH1726 device object
 *  @param Threshold_Low    Threshold low value
 *  @param Threshold_High   Threshold high value
 *  @param Latched          Set to #true to enable latched interrupts
 *  @param Persist          Interrupt persist value
 *  @return                 BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_EnableInterrupt(BH1726_t& p_Device, uint16_t Threshold_Low, uint16_t Threshold_High, bool Latched, uint8_t Persist);

/** @brief          Disable the sensor interrupts.
 *  @param p_Device BH1726 device object
 *  @return         BH1726_ERR_OK when successful
 */
BH1726_Error_t BH1726_DisableInterrupt(BH1726_t& p_Device);

#endif /* BH1726_H_ */