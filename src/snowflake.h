//
// Created by zynfly on 2018/9/4.
//

#ifndef SNOWFLAKE_SNOWFLAKE_H
#define SNOWFLAKE_SNOWFLAKE_H

#include <mutex>
#include <ctime>


class SnowFlake
{
public:
    explicit SnowFlake(
            uint64_t start_timestamp,
            uint32_t datacenter_id,
            uint32_t worker_id,
            uint8_t timestamp_bits,
            uint8_t datacenter_id_bits,
            uint8_t worker_id_bits,
            uint8_t sequence_bits
    );

    uint64_t nextId();

    uint64_t timeGen();

    uint64_t tilNextMillis(uint64_t lastTimestamp);

private:
    uint64_t _timestamp;
    uint32_t _sequence;
    uint32_t _datacenter_id;
    uint32_t _worker_id;
    uint64_t _start_timestamp;
    uint32_t _timestamp_bits;
    uint32_t _datacenter_id_bits;
    uint32_t _worker_id_bits;
    uint32_t _sequence_bits;
    uint32_t _timestamp_shift;
    uint32_t _datacenter_id_shift;
    uint32_t _worker_id_shift;
    uint32_t _max_datacenter_id;
    uint32_t _max_worker_id;
    uint32_t _max_sequence;

    std::mutex _mutex;
};


#endif //SNOWFLAKE_SNOWFLAKE_H
