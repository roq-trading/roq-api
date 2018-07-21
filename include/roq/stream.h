/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <algorithm>
#include <cstdarg>
#include <ostream>
#include <string>
#include <vector>

namespace roq {
namespace stream {
namespace details {

// BasicBufferWriter
// - write to pre-allocated buffer
template <size_t N>
class BasicBufferWriter {
 public:
  explicit BasicBufferWriter(char *buffer)
      : _begin(buffer),
        _pointer(buffer) {
  }
  BasicBufferWriter() = delete;
  BasicBufferWriter(const BasicBufferWriter&) = delete;
  void operator=(const BasicBufferWriter&) = delete;
  BasicBufferWriter& printf(const char *format, ...) {
      // TODO(thraneh): __attribute__((format(printf, 1, 2))) {
    va_list vargs;
    va_start(vargs, format);
    auto res = ::vsnprintf(_pointer, _length, format, vargs);
    va_end(vargs);
    advance(res);
    return *this;
  }
  BasicBufferWriter& strftime(const char *format, struct tm *tm) {
    auto res = ::strftime(_pointer, _length, format, tm);
    advance(res);
    return *this;
  }
  template <typename T>
  BasicBufferWriter& print(const T& value) {
    return write(*this, value);  // this function must exist
  }
  char *finish() {
    if (!_failed) {
      *_pointer = '\0';
      _pointer = nullptr;  // further actions will fail
      _length = 0;
      return _begin;
    } else {
      throw std::runtime_error("Print to buffer failed");
    }
  }
  void finish(std::string& string) {
    string.assign(finish());
  }
  std::ostream& finish(std::ostream& stream) {
    return stream << finish();
  }

 private:
  template <typename Res>
  void advance(Res res) {
    if (res > 0 && res < _length) {
      _pointer += res;
      _length -= res;
    } else {
      // TODO(thraneh): consider failing early instead...
      _pointer = nullptr;
      _length = 0;
      _failed = true;
    }
  }

 private:
  char *_begin;
  char *_pointer;
  size_t _length = N;
  bool _failed = false;
};  // BasicBufferWriter

// StackWriter
template <size_t N>
class StackWriter : public BasicBufferWriter<N> {
 public:
  StackWriter() : BasicBufferWriter<N>(_buffer) {}

 private:
  char _buffer[N];
};  // StackWriter

// BasicTimePointStr
class BasicTimePointStr {
 public:
  BasicTimePointStr() = delete;
  BasicTimePointStr(const BasicTimePointStr&) = delete;
  BasicTimePointStr(BasicTimePointStr&&) = default;
  void operator=(const BasicTimePointStr&) = delete;
  explicit BasicTimePointStr(uint64_t microseconds);
  const char *c_str() const {
    return _result;
  }

 private:
  char _result[27];  // YYYY-MM-DDTHH:MM:SS.UUUUUU\0
};  // BasicTimePointStr

// TimePointStr
template <typename T>
class TimePointStr final : public BasicTimePointStr {
 public:
  explicit TimePointStr(T time_point)
      : BasicTimePointStr(to_microseconds(time_point)) {
  }

 private:
  static uint64_t to_microseconds(T time_point) {
    auto epoch = std::chrono::time_point_cast<
        std::chrono::microseconds>(time_point).time_since_epoch();
    return std::chrono::duration_cast<
        std::chrono::microseconds>(epoch).count();
  }
};  // TimePointStr

}  // namespace details

// pre-populate cache
// - time-point string conversion
extern void warm_cache();

}  // namespace stream

// ...

template <typename T>
struct Vector {
  explicit Vector(const std::vector<T>& value) : _value(value) {}
  const std::vector<T>& _value;
};

template <typename T>
struct Set {
  explicit Set(const std::unordered_set<T>& value) : _value(value) {}
  const std::unordered_set<T>& _value;
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Vector<T> value);

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Set<T> value);

// enums

std::ostream& operator<<(std::ostream& stream, const ConnectionStatus value);
std::ostream& operator<<(std::ostream& stream, const GatewayStatus value);
std::ostream& operator<<(std::ostream& stream, const Side value);
std::ostream& operator<<(std::ostream& stream, const OrderType value);
std::ostream& operator<<(std::ostream& stream, const TimeInForce value);
std::ostream& operator<<(std::ostream& stream, const PositionEffect value);
std::ostream& operator<<(std::ostream& stream, const TradingStatus value);
std::ostream& operator<<(std::ostream& stream, const OrderStatus value);
std::ostream& operator<<(std::ostream& stream, const Layer& value);

// messages

std::ostream& operator<<(std::ostream& stream, const Handshake& value);
std::ostream& operator<<(std::ostream& stream, const HandshakeAck& value);
std::ostream& operator<<(std::ostream& stream, const Heartbeat& value);
std::ostream& operator<<(std::ostream& stream, const HeartbeatAck& value);
std::ostream& operator<<(std::ostream& stream, const DownloadBegin& value);
std::ostream& operator<<(std::ostream& stream, const DownloadEnd& value);
std::ostream& operator<<(std::ostream& stream, const MarketDataStatus& value);
std::ostream& operator<<(std::ostream& stream, const OrderManagerStatus& value);
std::ostream& operator<<(std::ostream& stream, const SessionStatistics& value);
std::ostream& operator<<(std::ostream& stream, const DailyStatistics& value);
std::ostream& operator<<(std::ostream& stream, const MarketByPrice& value);
std::ostream& operator<<(std::ostream& stream, const TradeSummary& value);
std::ostream& operator<<(std::ostream& stream, const ReferenceData& value);
std::ostream& operator<<(std::ostream& stream, const MarketStatus& value);
std::ostream& operator<<(std::ostream& stream, const PositionUpdate& value);
std::ostream& operator<<(std::ostream& stream, const OrderUpdate& value);
std::ostream& operator<<(std::ostream& stream, const TradeUpdate& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrder& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrder& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrder& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrderAck& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrderAck& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrderAck& value);

std::ostream& operator<<(std::ostream& stream, const MessageInfo& value);

// events

std::ostream& operator<<(std::ostream& stream, const ConnectionStatusEvent& value);

std::ostream& operator<<(std::ostream& stream, const BatchBeginEvent& value);
std::ostream& operator<<(std::ostream& stream, const BatchEndEvent& value);
std::ostream& operator<<(std::ostream& stream, const HandshakeEvent& value);
std::ostream& operator<<(std::ostream& stream, const HandshakeAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const HeartbeatEvent& value);
std::ostream& operator<<(std::ostream& stream, const HeartbeatAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const DownloadBeginEvent& value);
std::ostream& operator<<(std::ostream& stream, const DownloadEndEvent& value);
std::ostream& operator<<(std::ostream& stream, const MarketDataStatusEvent& value);
std::ostream& operator<<(std::ostream& stream, const OrderManagerStatusEvent& value);
std::ostream& operator<<(std::ostream& stream, const SessionStatisticsEvent& value);
std::ostream& operator<<(std::ostream& stream, const DailyStatisticsEvent& value);
std::ostream& operator<<(std::ostream& stream, const MarketByPriceEvent& value);
std::ostream& operator<<(std::ostream& stream, const TradeSummaryEvent& value);
std::ostream& operator<<(std::ostream& stream, const ReferenceDataEvent& value);
std::ostream& operator<<(std::ostream& stream, const MarketStatusEvent& value);
std::ostream& operator<<(std::ostream& stream, const PositionUpdateEvent& value);
std::ostream& operator<<(std::ostream& stream, const OrderUpdateEvent& value);
std::ostream& operator<<(std::ostream& stream, const TradeUpdateEvent& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrderEvent& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrderEvent& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrderEvent& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrderAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrderAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrderAckEvent& value);

}  // namespace roq
