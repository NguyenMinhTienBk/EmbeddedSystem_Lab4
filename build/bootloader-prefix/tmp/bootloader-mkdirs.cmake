# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.0.2/components/bootloader/subproject"
  "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader"
  "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader-prefix"
  "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader-prefix/tmp"
  "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader-prefix/src/bootloader-stamp"
  "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader-prefix/src"
  "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/OneDrive - m4n7/BK/HK231/EmbeddedSystem/esp-idf-ws/lab_04/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
