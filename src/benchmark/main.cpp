/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <benchmark/benchmark.h>

#include "roq/api.h"
#include "roq/codec.h"
#include "roq/stream.h"

#include "roq/utils.h"

namespace {
inline roq::MessageInfo create_message_info(
    std::chrono::system_clock::time_point now) {
  return roq::MessageInfo {
    .source = "femas",
    .source_seqno = 1234,
    .client_receive_time = now,
    .source_create_time = now,
    .source_receive_time = now,
    .origin_create_time = now,
    .routing_latency = std::chrono::milliseconds(1),
    .is_last = true,
    .channel = 1234,
  };
}
inline roq::SessionStatistics create_session_statistics(
    std::chrono::system_clock::time_point now) {
  return roq::SessionStatistics {
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .pre_open_interest = 123.45,
    .pre_settlement_price = 123.45,
    .pre_close_price = 123.45,
    .highest_traded_price = 123.45,
    .lowest_traded_price = 123.45,
    .upper_limit_price = 123.45,
    .lower_limit_price = 123.45,
    .exchange_time = now,
    .channel = 1234,
  };
}
inline roq::DailyStatistics create_daily_statistics(
    std::chrono::system_clock::time_point now) {
  return roq::DailyStatistics {
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .open_price = 123.45,
    .settlement_price = 123.45,
    .close_price = 123.45,
    .open_interest = 123.45,
    .exchange_time = now,
    .channel = 1234,
  };
}
inline roq::MarketByPrice create_market_by_price() {
  auto result = roq::MarketByPrice {
    .exchange = "CFFEX",
    .symbol = "IC1806",
  };
  for (auto i = 0; i < roq::MAX_DEPTH; ++i) {
    auto layer = result.depth[i];
    layer.bid_price = static_cast<double>(i * 4) + 0.1;
    layer.bid_quantity = static_cast<double>(i * 4 + 1) + 0.2;
    layer.ask_price = static_cast<double>(i * 4 + 2) + 0.3;
    layer.ask_price = static_cast<double>(i * 4 + 3) + 0.4;
  }
  result.total_bid_volume = 123.45;
  result.total_ask_volume = 123.45;
  result.channel = 1234;
  return result;
}
inline roq::TradeSummary create_trade_summary(
    std::chrono::system_clock::time_point now) {
  return roq::TradeSummary {
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .price = 123.45,
    .volume = 2345.67,
    .turnover = 1.4e10,
    .side = roq::Side::Buy,
    .exchange_time = now,
    .channel = 1234,
  };
}
inline roq::ReferenceData create_reference_data() {
  return roq::ReferenceData {
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .tick_size = 0.12345,
    .limit_up = 9876.5,
    .limit_down = 1234.5,
    .multiplier = 1.0e7,
  };
}
inline roq::MarketStatus create_market_status() {
  return roq::MarketStatus {
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .trading_status = roq::TradingStatus::Open,
  };
}
inline roq::PositionUpdate create_position_update() {
  return roq::PositionUpdate {
    .account = "abc123",
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .side = roq::Side::Buy,
    .last_trade_id = 234,
    .position = 123.45,
    .position_cost = 12345,
    .position_yesterday = 234.56,
    .position_cost_yesterday = 23456,
  };
}
inline roq::OrderUpdate create_order_update(
    std::chrono::system_clock::time_point now) {
  return roq::OrderUpdate {
    .account = "abc123",
    .order_id = 123,
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .order_status = roq::OrderStatus::Working,
    .side = roq::Side::Buy,
    .remaining_quantity = 123.45,
    .traded_quantity = 234.56,
    .position_effect = roq::PositionEffect::Close,
    .order_template = "default",
    .insert_time = now,
    .cancel_time = now,
    .order_local_id = 234,
    .order_external_id = "ext1234",
  };
}
inline roq::TradeUpdate create_trade_update(
    std::chrono::system_clock::time_point now) {
  return roq::TradeUpdate {
    .account = "abc123",
    .trade_id = 123,
    .order_id = 234,
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .side = roq::Side::Buy,
    .quantity = 123.45,
    .price = 234.56,
    .position_effect = roq::PositionEffect::Close,
    .order_template = "default",
    .trade_time = now,
    .order_external_id = "ext1234",
    .trade_external_id = "ext2345",
  };
}
inline roq::SourceInfo create_source_info(
    std::chrono::system_clock::time_point now) {
  return roq::SourceInfo {
    .seqno = 12345678,
    .create_time = now,
  };
}
inline roq::CreateOrder create_create_order() {
  return roq::CreateOrder {
    .account = "01234567",
    .order_id = 1234,
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .side = roq::Side::Buy,
    .quantity = 1234.5,
    .order_type = roq::OrderType::Market,
    .limit_price = 123.45,
    .time_in_force = roq::TimeInForce::IOC,
    .position_effect = roq::PositionEffect::Close,
    .order_template = "default",
  };
}
}  // namespace

// LogPrintFixture
class LogPrintFixture : public ::benchmark::Fixture {
 protected:
  roq::logging::detail::sink_t _sink = [](const char* message){};
};

// LogFileFixture
class LogFileFixture : public ::benchmark::Fixture {
 protected:
  // TODO(thraneh): add setup + tear-down code here
  roq::logging::detail::sink_t _sink = [](const char* message){
    LOG(INFO) << message;
  };
};

// PrintFixture
class PrintFixture : public ::benchmark::Fixture {
 protected:
  std::stringstream _ss;
  // externalize the access to system resources
  std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
};

// EncodeFixture
class EncodeFixture : public ::benchmark::Fixture {
 protected:
  // typical pattern is to re-use the same flatbuffers builder
  flatbuffers::FlatBufferBuilder _fbb;
  // externalize the access to system resources
  std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
};

// DecodeFixture
class DecodeFixture
    : public ::benchmark::Fixture,
      public roq::codec::EventHandler {
 protected:
  // typical pattern is to re-use the same flatbuffers builder
  flatbuffers::FlatBufferBuilder _fbb;
  std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
  const char *_source = "femas";
  roq::BatchInfo _batch_info {
    .seqno = 1234,
    .send_time = _now,
  };
  bool _is_first = false;
  bool _is_last = true;
  std::chrono::system_clock::time_point _receive_time = _now;
  uint64_t _count = 0;

 private:
  void on(const roq::BatchBeginEvent&) override { ++_count; }
  void on(const roq::BatchEndEvent&) override { ++_count; }
  void on(const roq::HandshakeEvent&) override { ++_count; }
  void on(const roq::HandshakeAckEvent&) override { ++_count; }
  void on(const roq::HeartbeatEvent&) override { ++_count; }
  void on(const roq::HeartbeatAckEvent&) override { ++_count; }
  void on(const roq::DownloadBeginEvent&) override { ++_count; }
  void on(const roq::DownloadEndEvent&) override { ++_count; }
  void on(const roq::MarketDataStatusEvent&) override { ++_count; }
  void on(const roq::OrderManagerStatusEvent&) override { ++_count; }
  void on(const roq::SessionStatisticsEvent&) override { ++_count; }
  void on(const roq::DailyStatisticsEvent&) override { ++_count; }
  void on(const roq::MarketByPriceEvent&) override { ++_count; }
  void on(const roq::TradeSummaryEvent&) override { ++_count; }
  void on(const roq::ReferenceDataEvent&) override { ++_count; }
  void on(const roq::MarketStatusEvent&) override { ++_count; }
  void on(const roq::PositionUpdateEvent&) override { ++_count; }
  void on(const roq::OrderUpdateEvent&) override { ++_count; }
  void on(const roq::TradeUpdateEvent&) override { ++_count; }
  void on(const roq::CreateOrderEvent&) override { ++_count; }
  void on(const roq::ModifyOrderEvent&) override { ++_count; }
  void on(const roq::CancelOrderEvent&) override { ++_count; }
  void on(const roq::CreateOrderAckEvent&) override { ++_count; }
  void on(const roq::ModifyOrderAckEvent&) override { ++_count; }
  void on(const roq::CancelOrderAckEvent&) override { ++_count; }
};

// date

void BM_Date_Print(
    benchmark::State& state) {
  std::vector<char> buffer(11);
  auto epoch = roq::utils::JulianDayNumber::from_calendar(1970, 1, 1);
  int offset = 0;
  for (auto _ : state) {
    roq::utils::JulianDayNumber::printf(
      &buffer.at(0),
      buffer.size(),
      epoch + (++offset % 36500));
    std::string tmp(&buffer[0]);
  }
}
BENCHMARK(BM_Date_Print);

// date-time

void BM_DateTime_Print(
    benchmark::State& state) {
  std::vector<char> buffer(27);
  auto epoch = roq::utils::JulianDayNumber::from_calendar(1970, 1, 1);
  int offset = 0;
  for (auto _ : state) {
    roq::utils::DateTime::printf(
      &buffer.at(0),
      buffer.size(),
      static_cast<uint64_t>(epoch + (++offset % 36500)) * 1000000 * 86400);
    std::string tmp(&buffer[0]);
  }
}
BENCHMARK(BM_DateTime_Print);

// memcpy

void BM_MemCpy(
    benchmark::State& state) {
  std::vector<uint8_t> source(1500);
  std::vector<uint8_t> destination(1024 * 1024 * 64);
  size_t offset = 0;
  for (auto _ : state) {
    std::memcpy(&destination[offset], &source[0], source.size());
    offset += 2048;
    if (destination.size() <= (offset + source.size()))
      offset = 0;
  }
}
BENCHMARK(BM_MemCpy);

// memmove

void BM_MemMove(
    benchmark::State& state) {
  std::vector<uint8_t> source(1500);
  std::vector<uint8_t> destination(1024 * 1024 * 64);
  size_t offset = 0;
  for (auto _ : state) {
    std::memmove(&destination[offset], &source[0], source.size());
    offset += 2048;
    if (destination.size() <= (offset + source.size()))
      offset = 0;
  }
}
BENCHMARK(BM_MemMove);

// stringstream

void BM_StringStream_Create(
    benchmark::State& state) {
  for (auto _ : state) {
    std::stringstream ss;
    auto str = ss.str();
  }
}
BENCHMARK(BM_StringStream_Create);


// LogMessage

BENCHMARK_DEFINE_F(LogPrintFixture, BM_LogMessage_Old)(
    benchmark::State& state) {
  auto sink = [](const char* message){};
  for (auto _ : state) {
    roq::logging::detail::LogMessage logger(
        __FILE__,
        ":" STRINGIZE(__LINE__) "] ",
        _sink);
    logger.stream() << "";
  }
}
BENCHMARK_REGISTER_F(LogPrintFixture, BM_LogMessage_Old);

BENCHMARK_DEFINE_F(LogPrintFixture, BM_LogMessage_New)(
    benchmark::State& state) {
  for (auto _ : state) {
    roq::logging::detail::LogMessage logger(
        __FILE__ ":" STRINGIZE(__LINE__) "] ",
        _sink);
    logger.stream() << "";
  }
}
BENCHMARK_REGISTER_F(LogPrintFixture, BM_LogMessage_New);

BENCHMARK_DEFINE_F(LogPrintFixture, BM_LogMessage_Print)(
    benchmark::State& state) {
  auto sink = [](const char* message){};
  for (auto _ : state) {
    roq::logging::detail::LogMessage logger(
        __FILE__ ":" STRINGIZE(__LINE__) "] ",
        _sink);
    logger.stream() <<
      "abcdefghij" << "klmnopqrst" << "uvwxyz" <<
      "ABCDEFGHIJ" << "KLMNOPQRST" << "UVWXYZ" <<
      "0123456789";
  }
}
BENCHMARK_REGISTER_F(LogPrintFixture, BM_LogMessage_Print);

BENCHMARK_DEFINE_F(LogFileFixture, BM_LogMessage_Print)(
    benchmark::State& state) {
  roq::logging::Logger::initialize(false, "/tmp");
  for (auto _ : state) {
    LOG(INFO) << "";
  }
  roq::logging::Logger::shutdown();
}
BENCHMARK_REGISTER_F(LogFileFixture, BM_LogMessage_Print);


// MessageInfo
// -- note! no interface to call stand-alone encode/decode

BENCHMARK_DEFINE_F(PrintFixture, BM_MessageInfo_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    _ss.str("");
    _ss << message_info;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_MessageInfo_Print);


// SessionStatistics

BENCHMARK_DEFINE_F(EncodeFixture, BM_SessionStatistics_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto session_statistics = create_session_statistics(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            session_statistics));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_SessionStatistics_Encode);


BENCHMARK_DEFINE_F(PrintFixture, BM_SessionStatistics_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto session_statistics = create_session_statistics(_now);
    _ss.str("");
    _ss << session_statistics;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_SessionStatistics_Print);


// DailyStatistics

BENCHMARK_DEFINE_F(EncodeFixture, BM_DailyStatistics_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto daily_statistics = create_daily_statistics(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            daily_statistics));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_DailyStatistics_Encode);


BENCHMARK_DEFINE_F(PrintFixture, BM_DailyStatistics_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto daily_statistics = create_daily_statistics(_now);
    _ss.str("");
    _ss << daily_statistics;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_DailyStatistics_Print);


// MarketByPrice

BENCHMARK_DEFINE_F(EncodeFixture, BM_MarketByPrice_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto market_by_price = create_market_by_price();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            market_by_price));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_MarketByPrice_Encode);


BENCHMARK_DEFINE_F(PrintFixture, BM_MarketByPrice_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto market_by_price = create_market_by_price();
    _ss.str("");
    _ss << market_by_price;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_MarketByPrice_Print);


// TradeSummary

BENCHMARK_DEFINE_F(EncodeFixture, BM_TradeSummary_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto trade_summary = create_trade_summary(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            trade_summary));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_TradeSummary_Encode);

BENCHMARK_DEFINE_F(PrintFixture, BM_TradeSummary_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto trade_summary = create_trade_summary(_now);
    _ss.str("");
    _ss << trade_summary;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_TradeSummary_Print);


// ReferenceData

BENCHMARK_DEFINE_F(EncodeFixture, BM_ReferenceData_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto reference_data = create_reference_data();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            reference_data));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_ReferenceData_Encode);

BENCHMARK_DEFINE_F(PrintFixture, BM_ReferenceData_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto reference_data = create_reference_data();
    _ss.str("");
    _ss << reference_data;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_ReferenceData_Print);


// MarketStatus

BENCHMARK_DEFINE_F(EncodeFixture, BM_MarketStatus_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto market_status = create_market_status();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            market_status));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_MarketStatus_Encode);

BENCHMARK_DEFINE_F(PrintFixture, BM_MarketStatus_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto market_status = create_market_status();
    _ss.str("");
    _ss << market_status;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_MarketStatus_Print);


// PositionUpdate

BENCHMARK_DEFINE_F(EncodeFixture, BM_PositionUpdate_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto position_update = create_position_update();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            position_update));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_PositionUpdate_Encode);

BENCHMARK_DEFINE_F(PrintFixture, BM_PositionUpdate_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto position_update = create_position_update();
    _ss.str("");
    _ss << position_update;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_PositionUpdate_Print);


// OrderUpdate

BENCHMARK_DEFINE_F(EncodeFixture, BM_OrderUpdate_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto order_update = create_order_update(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            order_update));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_OrderUpdate_Encode);

BENCHMARK_DEFINE_F(PrintFixture, BM_OrderUpdate_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto order_update = create_order_update(_now);
    _ss.str("");
    _ss << order_update;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_OrderUpdate_Print);


// TradeUpdate

BENCHMARK_DEFINE_F(EncodeFixture, BM_TradeUpdate_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto trade_update = create_trade_update(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            trade_update));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_TradeUpdate_Encode);

BENCHMARK_DEFINE_F(PrintFixture, BM_TradeUpdate_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto trade_update = create_trade_update(_now);
    _ss.str("");
    _ss << trade_update;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_TradeUpdate_Print);


// CreateOrder

BENCHMARK_DEFINE_F(EncodeFixture, BM_CreateOrder_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto crate_order = create_create_order();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            crate_order));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_CreateOrder_Encode);

BENCHMARK_DEFINE_F(PrintFixture, BM_CreateOrder_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto create_order = create_create_order();
    _ss.str("");
    _ss << create_order;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_CreateOrder_Print);


// SessionStatisticsEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_SessionStatisticsEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto session_statistics = create_session_statistics(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            session_statistics));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_SessionStatisticsEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_SessionStatisticsEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto session_statistics = create_session_statistics(_now);
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          session_statistics));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_SessionStatisticsEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_SessionStatisticsEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto session_statistics = create_session_statistics(_now);
    auto session_statistics_event = roq::SessionStatisticsEvent {
        .message_info = message_info,
        .session_statistics = session_statistics
    };
    _ss.str("");
    _ss << session_statistics_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_SessionStatisticsEvent_Print);


// DailyStatisticsEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_DailyStatisticsEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto daily_statistics = create_daily_statistics(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            daily_statistics));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_DailyStatisticsEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_DailyStatisticsEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto daily_statistics = create_daily_statistics(_now);
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          daily_statistics));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_DailyStatisticsEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_DailyStatisticsEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto daily_statistics = create_daily_statistics(_now);
    auto daily_statistics_event = roq::DailyStatisticsEvent {
        .message_info = message_info,
        .daily_statistics = daily_statistics
    };
    _ss.str("");
    _ss << daily_statistics_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_DailyStatisticsEvent_Print);


// MarketByPriceEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_MarketByPriceEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto market_by_price = create_market_by_price();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            market_by_price));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_MarketByPriceEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_MarketByPriceEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto market_by_price = create_market_by_price();
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          market_by_price));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_MarketByPriceEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_MarketByPriceEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto market_by_price = create_market_by_price();
    auto market_by_price_event = roq::MarketByPriceEvent {
        .message_info = message_info,
        .market_by_price = market_by_price
    };
    _ss.str("");
    _ss << market_by_price_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_MarketByPriceEvent_Print);


// TradeSummaryEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_TradeSummaryEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto trade_summary = create_trade_summary(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            trade_summary));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_TradeSummaryEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_TradeSummaryEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto trade_summary = create_trade_summary(_now);
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          trade_summary));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_TradeSummaryEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_TradeSummaryEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto trade_summary = create_trade_summary(_now);
    auto trade_summary_event = roq::TradeSummaryEvent {
        .message_info = message_info,
        .trade_summary = trade_summary
    };
    _ss.str("");
    _ss << trade_summary_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_TradeSummaryEvent_Print);


// ReferenceDataEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_ReferenceDataEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto reference_data = create_reference_data();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            reference_data));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_ReferenceDataEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_ReferenceDataEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto reference_data = create_reference_data();
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          reference_data));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_ReferenceDataEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_ReferenceDataEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto reference_data = create_reference_data();
    auto reference_data_event = roq::ReferenceDataEvent {
        .message_info = message_info,
        .reference_data = reference_data
    };
    _ss.str("");
    _ss << reference_data_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_ReferenceDataEvent_Print);


// MarketStatusEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_MarketStatusEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto market_status = create_market_status();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            market_status));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_MarketStatusEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_MarketStatusEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto market_status = create_market_status();
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          market_status));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_MarketStatusEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_MarketStatusEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto market_status = create_market_status();
    auto market_status_event = roq::MarketStatusEvent {
        .message_info = message_info,
        .market_status = market_status
    };
    _ss.str("");
    _ss << market_status_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_MarketStatusEvent_Print);


// PositionUpdateEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_PositionUpdateEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto position_update = create_position_update();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            position_update));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_PositionUpdateEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_PositionUpdateEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto position_update = create_position_update();
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          position_update));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_PositionUpdateEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_PositionUpdateEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto position_update = create_position_update();
    auto position_update_event = roq::PositionUpdateEvent {
        .message_info = message_info,
        .position_update = position_update
    };
    _ss.str("");
    _ss << position_update_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_PositionUpdateEvent_Print);


// OrderUpdateEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_OrderUpdateEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto order_update = create_order_update(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            order_update));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_OrderUpdateEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_OrderUpdateEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto order_update = create_order_update(_now);
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          order_update));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_OrderUpdateEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_OrderUpdateEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto order_update = create_order_update(_now);
    auto order_update_event = roq::OrderUpdateEvent {
        .message_info = message_info,
        .order_update = order_update
    };
    _ss.str("");
    _ss << order_update_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_OrderUpdateEvent_Print);


// TradeUpdateEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_TradeUpdateEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto trade_update = create_trade_update(_now);
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            trade_update));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_TradeUpdateEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_TradeUpdateEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto trade_update = create_trade_update(_now);
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          trade_update));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_TradeUpdateEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_TradeUpdateEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto trade_update = create_trade_update(_now);
    auto trade_update_event = roq::TradeUpdateEvent {
        .message_info = message_info,
        .trade_update = trade_update
    };
    _ss.str("");
    _ss << trade_update_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_TradeUpdateEvent_Print);


// CreateOrderEvent

BENCHMARK_DEFINE_F(EncodeFixture, BM_CreateOrderEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto create_order = create_create_order();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            create_order));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_CreateOrderEvent_Encode);

// HANS -- need a templated fixture (lambda functions to create/decode)
BENCHMARK_DEFINE_F(DecodeFixture, BM_CreateOrderEvent_Decode)(
    benchmark::State& state) {
  // encode (outside loop)
  auto source_info = create_source_info(_now);
  auto create_order = create_create_order();
  _fbb.Finish(
      roq::codec::convert2(
          _fbb,
          source_info,
          create_order));
  auto buffer = _fbb.GetBufferPointer();
  auto length = _fbb.GetSize();
  roq::codec::EventDispatcher dispatcher(*this);
  // decode
  for (auto _ : state) {
    dispatcher.dispatch(
        buffer,
        length,
        _source,
        _batch_info,
        _is_first,
        _is_last,
        _receive_time);
  }
  assert(_count > 0);
}
BENCHMARK_REGISTER_F(DecodeFixture, BM_CreateOrderEvent_Decode);

BENCHMARK_DEFINE_F(PrintFixture, BM_CreateOrderEvent_Print)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto message_info = create_message_info(_now);
    auto create_order = create_create_order();
    auto create_order_event = roq::CreateOrderEvent {
        .message_info = message_info,
        .create_order = create_order
    };
    _ss.str("");
    _ss << create_order_event;
    auto str = _ss.str();
  }
}
BENCHMARK_REGISTER_F(PrintFixture, BM_CreateOrderEvent_Print);


// main

BENCHMARK_MAIN();
