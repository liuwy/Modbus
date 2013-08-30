/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mb.h,v 1.17 2006/12/07 22:10:34 wolti Exp $
 */

/*********************************************************************
 * 版权所有： 北京力控华康科技有限公司版权所有 Copyright ©2009-2013
 * 文件名称： MB.h
 * 文件标识： Modbus协议栈
 * 内容摘要： Modbus协议栈接口说明。
 * 当前版本： 1.0.0.1
 * 作   者： 华康研发部 刘文跃
 * 完成日期： 2013-8-29
 *
 * 修改日期        版本号     修改人         修改内容
 * ---------------------------------------------------------------
 * 2013/08/29    V1.0     刘文跃          初始创建
 *********************************************************************/

#ifndef _MB_H
#define _MB_H

#ifdef __cplusplus
 extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/

/** @ingroup enum_gtoup
 * @brief Modbus TCP 默认使用端口(502)
 */

#define MB_TCP_PORT_USE_DEFAULT 0

/* ----------------------- Type definitions ---------------------------------*/

/** @ingroup enum_gtoup
 * @brief Modbus 主从模式 (Master/Slave).
 *
 * Modbus 协议栈有两种实现,Maseter或Slave. Master发送请求命令,接收应答命令.
 * 从站接收请求命令,发送应答命令.
 */

typedef enum
{
    MB_MASTER,                  /**< 主站模式.*/
    MB_SLAVE                    /**< 从栈模式. */
} eMB_MSMode;

/** @ingroup enum_gtoup
 * @brief Modbus 串行传输模式 (RTU/ASCII).
 *
 * Modbus 支持两种传输模式, ASCII 或 RTU. RTU传输速度较快,但是对硬件更多的要求同时要求网络
 * 低噪音. ASCII传输速度较慢但是更可靠.
 */

 typedef enum
{
    MB_RTU,                     /**< RTU 传输模式. */
    MB_ASCII,                   /**< ASCII 传输模式. */
    MB_TCP                      /**< TCP 传输模式. */
} eMBMode;

/** @ingroup enum_gtoup
 * @brief 对寄存器的读写操作.
 */
typedef enum
{
    MB_REG_READ,                /**< 读寄存器的值. */
    MB_REG_WRITE                /**< 修改寄存器的值. */
} eMBRegisterMode;

/** @ingroup enum_gtoup
 * @brief  Slave 错误码 eMBExceptionCode .
 */

typedef enum
{
    ILLEGAL_FUNCTION=1,          /**< 非法的功能码. */
    ILLEGAL_DATA_ADDRESS,        /**< 非法的数据地址. */
    ILLEGAL_DATA_VALUE,          /**< 非法的数据值. */
    SLAVE_DEVICE_FAILURE,        /**< 从设备故障. */
    ACKNOWLEDGE,                 /**< 确认. */
    SLAVE_DEVICE_BUSY,           /**< 从栈设备繁忙. */
    MEMORY_PARITY_ERROR,         /**< 奇偶校验错误. */
    GATEWAY_PATH_UNAVAILABLE,    /**< 网关路径不可用. */
    GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND      /**< 目标设备无法响应. */
} eMBExceptionCode;

/** @ingroup enum_gtoup
 * @brief 所有协议栈函数的返回值使用 Errorcodes.
 */
typedef enum
{
    MB_ENOERR,                  /**< 没有错误. */
    MB_ENOREG,                  /**< 非法的寄存器地址. */
    MB_EINVAL,                  /**< 非法参数. */
    MB_ENORES,                  /**< insufficient resources. */
    MB_EIO,                     /**< I/O 错误. */
    MB_EILLSTATE               /**< 协议栈错误的状态. */
} eMBErrorCode;

/* ----------------------- Parameter prototypes -----------------------------*/

/** @brief eMBInitPara 协议栈初始化输入参数.
 */
typedef struct
{
    eMB_MSMode msMode;          /**< 主从模式 */
    eMBMode    Mode;            /**< 通讯模式 */
    unsigned char ucSlaveAddress; /**<  协议栈地址 */
} eMBInitPara;

/* ----------------------- Function prototypes ------------------------------*/

/** @brief 初始化Modbus协议栈.
 *
 * @param [in] eInitPara 初始化参数
 *      - @ref eMB_MSMode "eInitPara::msMode 主从模式"
 *      - @ref eMBMode "eInitPara::Mode 通讯模式"
 *      - eInitPara.ucSlaveAddress 协议栈地址
 * @return 如果没有错误这个函数将返回 eMBErrorCode::MB_ENOERR.
 *  协议栈首先调用的函数
 *      - eMBErrorCode::MB_EINVAL 枚举类型不存在的数据.从站地址无效,
 *  有效地址在 1-127范围内.
 *      - eMBErrorCode::MB_EPORTERR 枚举类型不存在的数据.从站地址无效,
 */
eMBErrorCode    eMBInit( eMBInitPara eInitPara);

/** @brief 释放协议栈资源.
 * 这个函数将释放协议栈所有资源,它不需是协议栈最后被调用的函数.
 *
 * @return 如果资源被正确释放,它将返回 eMBErrorCode::MB_ENOERR.
 *   如果协议栈没有关闭,它将返回 eMBErrorCode::MB_EILLSTATE.
 */
eMBErrorCode    eMBClose( void );

#ifdef __cplusplus
 }
#endif
#endif // _MB_H
