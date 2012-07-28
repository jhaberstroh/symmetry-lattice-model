set(Metropolis_Growth_src site.cpp lattice.cpp interaction.cpp montecarlo.cpp filehandler.cpp)

add_library(Metropolis_Growth STATIC ${Metropolis_Growth_src})