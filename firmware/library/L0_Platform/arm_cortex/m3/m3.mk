SYSTEM_INCLUDES += $(LIB_DIR)/L0_Platform/arm_cortex/m3/ARM_CM3/
COMMON_FLAGS += -mcpu=cortex-m3 -mthumb -mfloat-abi=soft \
                 -fabi-version=0 -mtpcs-frame -mtpcs-leaf-frame
