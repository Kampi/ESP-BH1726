#include "bh1726.h"

#define BH1726_CMD_REGISTER_ADDR            (0x04 << 5)
#define BH1726_CMD_REGISTER_INT_RESET       0xE1
#define BH1726_CMD_REGISTER_SW_RESET        0xE4

#define BH1726_REG_CONTROL                  (BH1726_CMD_REGISTER_ADDR | 0x00)
#define BH1726_REG_TIMING                   (BH1726_CMD_REGISTER_ADDR | 0x01)
#define BH1726_REG_INTERRUPT                (BH1726_CMD_REGISTER_ADDR | 0x02)
#define BH1726_REG_TH_LOW_LOWER             (BH1726_CMD_REGISTER_ADDR | 0x03)
#define BH1726_REG_TH_LOW_UPPER             (BH1726_CMD_REGISTER_ADDR | 0x04)
#define BH1726_REG_TH_HIGH_LOWER            (BH1726_CMD_REGISTER_ADDR | 0x05)
#define BH1726_REG_TH_HIGH_UPPER            (BH1726_CMD_REGISTER_ADDR | 0x06)
#define BH1726_REG_GAIN                     (BH1726_CMD_REGISTER_ADDR | 0x07)
#define BH1726_REG_PART_ID                  (BH1726_CMD_REGISTER_ADDR | 0x12)
#define BH1726_REG_DATA0_LOWER              (BH1726_CMD_REGISTER_ADDR | 0x14)
#define BH1726_REG_DATA0_UPPER              (BH1726_CMD_REGISTER_ADDR | 0x15)
#define BH1726_REG_DATA1_LOWER              (BH1726_CMD_REGISTER_ADDR | 0x16)
#define BH1726_REG_DATA1_UPPER              (BH1726_CMD_REGISTER_ADDR | 0x17)
#define BH1726_REG_WAIT                     (BH1726_CMD_REGISTER_ADDR | 0x18)

/** @brief          Modify the content of a register.
 *  @param p_Device BH1726 device object
 *  @param Port     Register address
 *  @param Mask     Bit mask
 *  @param Level    Bit level
 *  @return         BH1726_ERR_OK when successful
 */
static BH1726_Error_t BH1726_ModifyRegister(BH1726_t& p_Device, uint8_t Address, uint8_t Mask, uint8_t Value)
{
    uint8_t Data[2] = { Address, 0x00 };
    BH1726_Error_t Error;

    if(p_Device.isInitialized == false)
    {
        return BH1726_ERR_NOT_INITIALIZED;
    }

    Error = p_Device.I2C_Write(p_Device.DevAddr, &Data[0], 1) | p_Device.I2C_Read(p_Device.DevAddr, &Data[1], 1);
    if(Error)
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    Data[1] &= ~Mask;
    Data[1] |= Value;

    if(p_Device.I2C_Write(p_Device.DevAddr, Data, sizeof(Data)))
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    return BH1726_ERR_OK;
}

BH1726_Error_t BH1726_Init(BH1726_t& p_Device)
{
    uint8_t Buffer[2];

    if((p_Device.I2C_Read == NULL) || (p_Device.I2C_Write == NULL) || (p_Device.Delay == NULL))
    {
        return BH1726_ERR_INVALID_ARG;
    }

    p_Device.isInitialized = false;

    Buffer[0] = BH1726_CMD_REGISTER_SW_RESET;
    if(p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0)
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    p_Device.Delay(100);

    Buffer[0] = BH1726_REG_PART_ID;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 1) != 0))
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    if(Buffer[1] != BH1726_PRODUCT_ID)
    {
        return BH1726_ERR_NO_DEVICE;
    }

    p_Device.ProductID = Buffer[1];

    Buffer[0] = BH1726_REG_CONTROL;
    Buffer[1] = (0x00 << 1) | (0x00 << 0);
    if(p_Device.I2C_Write(p_Device.DevAddr, Buffer, 2) != 0)
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    p_Device.isInterruptActive = false;
    p_Device.isPowerDown = true;
    p_Device.isInitialized = true;

    return BH1726_SetIntegrationTime(p_Device, 0xDA);
}

BH1726_Error_t BH1726_Deinit(BH1726_t& p_Device)
{
    if(p_Device.isInitialized == false)
    {
        return BH1726_ERR_OK;
    }

    BH1726_ERROR_CHECK(BH1726_PowerDown(p_Device, true));

    p_Device.isInitialized = false;

    return BH1726_ERR_OK;
}

BH1726_Error_t BH1726_PowerDown(BH1726_t& p_Device, bool Enable)
{    
    BH1726_ERROR_CHECK(BH1726_ModifyRegister(p_Device, BH1726_REG_CONTROL, (0x01 << 1) | (0x01 << 0), ((!Enable) << 1) | ((!Enable) << 0)));

    p_Device.isPowerDown = Enable;

    return BH1726_ERR_OK;
}

BH1726_Error_t BH1726_SetIntegrationTime(BH1726_t& p_Device, BH1726_Gain_t Gain0, BH1726_Gain_t Gain1)
{
    uint8_t Buffer[2];

    if(p_Device.isInitialized == false)
    {
        return BH1726_ERR_NOT_INITIALIZED;
    }

    Buffer[0] = BH1726_REG_GAIN;
    Buffer[1] = ((Gain1 & 0x03) << 2) | (Gain0 & 0x03);
    if(p_Device.I2C_Write(p_Device.DevAddr, Buffer, 2) != 0)
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    return BH1726_ERR_OK;
}

BH1726_Error_t BH1726_SetIntegrationTime(BH1726_t& p_Device, uint8_t Time)
{
    uint8_t Buffer[2];

    if(p_Device.isInitialized == false)
    {
        return BH1726_ERR_NOT_INITIALIZED;
    }

    Buffer[0] = BH1726_REG_TIMING;
    Buffer[1] = Time;
    if(p_Device.I2C_Write(p_Device.DevAddr, Buffer, 2) != 0)
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    return BH1726_ERR_OK;
}

BH1726_Error_t BH1726_Measure(BH1726_t& p_Device, uint16_t* p_Result, bool UseD1)
{
    uint8_t Time;
    uint8_t Buffer[3];
    bool PutBackToLowPower = false;

    if(p_Result == NULL)
    {
        return BH1726_ERR_INVALID_ARG;
    }
    else if(p_Device.isInitialized == false)
    {
        return BH1726_ERR_NOT_INITIALIZED;
    }
    else if(p_Device.isPowerDown)
    {
        BH1726_ERROR_CHECK(BH1726_PowerDown(p_Device, false));
        PutBackToLowPower = true;
    }

    Buffer[0] = BH1726_REG_TIMING;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Time, 1) != 0))
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    p_Device.Delay(Time);

    do
    {
        Buffer[0] = BH1726_REG_CONTROL;
        if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 2) != 0))
        {
            return BH1726_ERR_INVALID_RESPONSE;
        }

        p_Device.Delay(100);
    } while(((Buffer[1] >> 4) & 0x01) == false);

    Buffer[0] = BH1726_REG_DATA0_LOWER;
    if(UseD1)
    {
        Buffer[0] = BH1726_REG_DATA1_LOWER;
    }

    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 2) != 0))
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    *p_Result = (static_cast<uint16_t>(Buffer[2]) << 8) | Buffer[1];

    if(PutBackToLowPower)
    {
        BH1726_ERROR_CHECK(BH1726_PowerDown(p_Device, true));
    }

    return BH1726_ERR_OK;
}

BH1726_Error_t BH1726_EnableInterrupt(BH1726_t& p_Device, uint16_t Threshold_Low, uint16_t Threshold_High, bool Latched, uint8_t Persist)
{
    uint8_t Buffer[3];

    if(p_Device.isInitialized == false)
    {
        return BH1726_ERR_NOT_INITIALIZED;
    }

    Buffer[0] = BH1726_REG_TH_LOW_LOWER;
    Buffer[1] = Threshold_Low & 0xFF;
    Buffer[2] = (Threshold_Low >> 0x08) & 0xFF;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 2) != 0))
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    Buffer[0] = BH1726_REG_TH_HIGH_LOWER;
    Buffer[1] = Threshold_High & 0xFF;
    Buffer[2] = (Threshold_High >> 0x08) & 0xFF;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 2) != 0))
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    Buffer[0] = BH1726_REG_INTERRUPT;
    Buffer[1] = (Latched << 5) | (0x01 << 4) | (Persist & 0x0F);
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 1) != 0))
    {
        return BH1726_ERR_INVALID_RESPONSE;
    }

    return BH1726_ERR_OK;
}

BH1726_Error_t BH1726_DisableInterrupt(BH1726_t& p_Device)
{
    if(p_Device.isInitialized == false)
    {
        return BH1726_ERR_NOT_INITIALIZED;
    }
    else if(p_Device.isInterruptActive == false)
    {
        return BH1726_ERR_OK;
    }

    return BH1726_ModifyRegister(p_Device, BH1726_REG_INTERRUPT, (0x01 << 4), 0);
}