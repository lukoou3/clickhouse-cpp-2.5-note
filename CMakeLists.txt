cmake_minimum_required(VERSION 3.22)
project(clickhouse_cpp_2_5)

set(CMAKE_CXX_STANDARD 17)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(link_libs lz4)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(link_libs liblz4.dll ws2_32.lib)
ENDIF()

include_directories(. cityhash)

SET ( clickhouse-cpp-lib-src
        clickhouse/base/compressed.cpp
        clickhouse/base/input.cpp
        clickhouse/base/output.cpp
        clickhouse/base/platform.cpp
        clickhouse/base/socket.cpp
        clickhouse/base/wire_format.cpp
        clickhouse/base/endpoints_iterator.cpp

        clickhouse/columns/array.cpp
        clickhouse/columns/column.cpp
        clickhouse/columns/date.cpp
        clickhouse/columns/decimal.cpp
        clickhouse/columns/enum.cpp
        clickhouse/columns/factory.cpp
        clickhouse/columns/geo.cpp
        clickhouse/columns/ip4.cpp
        clickhouse/columns/ip6.cpp
        clickhouse/columns/lowcardinality.cpp
        clickhouse/columns/nullable.cpp
        clickhouse/columns/numeric.cpp
        clickhouse/columns/map.cpp
        clickhouse/columns/string.cpp
        clickhouse/columns/tuple.cpp
        clickhouse/columns/uuid.cpp

        clickhouse/columns/itemview.cpp

        clickhouse/types/type_parser.cpp
        clickhouse/types/types.cpp

        clickhouse/block.cpp
        clickhouse/client.cpp
        clickhouse/query.cpp

        # Headers
        )

SET ( cityhash-lib-src
        cityhash/city.cc
        )

add_executable(clickhouse_cpp_2_5 main.cpp ${clickhouse-cpp-lib-src} ${cityhash-lib-src} )



target_link_libraries(clickhouse_cpp_2_5 ${link_libs})
