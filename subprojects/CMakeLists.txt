cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(mcarloGUI_exe)

#Recursively explores subdirectories for cmake files and compliles them in order
add_subdirectory(Metropolis_Growth)
# add_subdirectory(ModelQT_GUI)

#Not needed until I make something which connects the two sources...
# set(mcarlo_source_files main.cpp)