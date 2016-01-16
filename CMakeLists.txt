cmake_minimum_required(VERSION 2.8)
project(chloe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
        CLSensors.h
        CLDisplay.h
        CLMotors.h
        vendor/sparklibs.h
        vendor/imu/SFE_LSM9DS0.h
        vendor/pwm/SFE_PCA9685.h
        vendor/oled/Edison_OLED.h
        vendor/spi/spi.h
        vendor/gpio/gpio.h
        vendor/gpio/gpio_edison.h
        vendor/ahrs/MadgwickAHRS.h
        math/CLConstants.h
        math/CLMath.h
        math/CLMatrix.h
        math/CLMatrixQuaternion.h
        math/CLQuaternion.h
        math/CLSIMD.h
        math/CLVector.h)

set(SOURCE_FILES
        main.cpp
        CLSensors.cpp
        CLDisplay.cpp
        CLMotors.cpp
        vendor/imu/SFE_LSM9DS0.cpp
        vendor/pwm/SFE_PCA9685.cpp
        vendor/oled/Edison_OLED.cpp
        vendor/spi/spi_device_edison.cpp
        vendor/spi/spi_port_edison.cpp
        vendor/gpio/gpio_edison.cpp
        vendor/ahrs/MadgwickAHRS.cpp
        math/CLMath.cpp)

add_executable(chloe ${SOURCE_FILES})
target_link_libraries(chloe mraa)
