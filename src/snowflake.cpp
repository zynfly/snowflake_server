//
// Created by zynfly on 2018/9/4.
//

#include "snowflake.h"
#include <iostream>
#include <chrono>

SnowFlake::SnowFlake(
        uint64_t start_timestamp,
        uint32_t datacenter_id,
        uint32_t worker_id,
        uint8_t timestamp_bits,
        uint8_t datacenter_id_bits,
        uint8_t worker_id_bits,
        uint8_t sequence_bits
) :
        _start_timestamp(start_timestamp),
        _datacenter_id(datacenter_id),
        _worker_id(worker_id),
        _timestamp_bits(timestamp_bits),
        _datacenter_id_bits(datacenter_id_bits),
        _worker_id_bits(worker_id_bits),
        _sequence_bits(sequence_bits)
{
    _max_datacenter_id = (uint32_t) (0xFFFFFFFF ^ (0xFFFFFFFF << _datacenter_id_bits));
    _max_worker_id     = (uint32_t) (0xFFFFFFFF ^ (0xFFFFFFFF << _worker_id_bits));
    _max_sequence      = (uint32_t) (0xFFFFFFFF ^ (0xFFFFFFFF << _sequence_bits));

    _worker_id_shift     = _sequence_bits;
    _datacenter_id_shift = _worker_id_bits + _worker_id_shift;
    _timestamp_shift     = _datacenter_id_bits + _datacenter_id_shift;
}

uint64_t SnowFlake::nextId()
{
    uint64_t now_timestamp = 0;
    uint64_t sequence      = 0;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        now_timestamp = timeGen();
        if (now_timestamp == _timestamp) {
            sequence = ++_sequence;
            if (sequence > _max_sequence) {
                sequence      = _sequence  = 0;
                now_timestamp = _timestamp = tilNextMillis(now_timestamp);
            }
        } else if (now_timestamp > _timestamp) {
            _timestamp = now_timestamp;
            _sequence  = 0;
        } else {
            return 0;
        }
    }

    return ((now_timestamp - _start_timestamp) << _timestamp_shift) |
           _datacenter_id << _datacenter_id_shift |
           _worker_id << _worker_id_shift |
           sequence;
}


uint64_t SnowFlake::timeGen()
{
    return (uint64_t) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
            .count();
}

uint64_t SnowFlake::tilNextMillis(uint64_t lastTimestamp)
{
    uint64_t timestamp = timeGen();
    while (timestamp <= lastTimestamp) {
        timestamp = timeGen();
    }
    return timestamp;
}