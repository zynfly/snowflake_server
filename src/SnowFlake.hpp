//
// Created by zynfly on 2018/9/4.
//

#ifndef SNOWFLAKE_SNOWFLAKE_H
#define SNOWFLAKE_SNOWFLAKE_H

#include <mutex>
#include <chrono>


class SnowFlakeInterface
{
public:
    virtual uint64_t nextId() = 0;

    virtual ~SnowFlakeInterface() = default;
};

template<typename SPAN>
class SnowFlake : public SnowFlakeInterface
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


    uint64_t nextId() override
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
                    now_timestamp = _timestamp = tilNextSpan(now_timestamp);
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

    uint64_t timeGen()
    {
        return static_cast<uint64_t>(std::chrono::duration_cast<SPAN>(std::chrono::steady_clock::now().time_since_epoch())
                .count());
    }

    uint64_t tilNextSpan(uint64_t lastTimestamp)
    {
        uint64_t timestamp = timeGen();
        while (timestamp <= lastTimestamp) {
            timestamp = timeGen();
        }
        return timestamp;
    }

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


class SecSnowFlake : public SnowFlake<std::chrono::seconds>
{
public:
    using SnowFlake::SnowFlake;
};

class MilSnowFlake : public SnowFlake<std::chrono::milliseconds>
{
public:
    using SnowFlake::SnowFlake;
};


#endif //SNOWFLAKE_SNOWFLAKE_H
