
#include "utils.h"
#include "../components/tion-api/log.h"
#include "../components/tion-api/tion-api-lt.h"
#include "test_api.h"
using namespace dentra::tion;

static std::vector<uint8_t> wr_data_;

class ApiLtTest {
  using this_type = ApiLtTest;

 public:
  TionApiLt api;
  explicit ApiLtTest(TestTionBleLtProtocol *protocol) {
    protocol->reader.set<ApiLtTest, &ApiLtTest::read_frame>(*this);
    protocol->writer.set<ApiLtTest, &ApiLtTest::write_data>(*this);

    this->api.writer.set<TionBleLtProtocol, &TionBleLtProtocol::write_frame>(*protocol);
  }

  bool read_frame(uint16_t type, const void *data, size_t size) { return this->api.read_frame(type, data, size); }

  bool write_data(const uint8_t *data, size_t size) {
    LOGD("Writting data: %s", hexencode(data, size).c_str());
    wr_data_.insert(wr_data_.end(), data, data + size);
    return true;
  }
};

bool test_api_lt(bool print) {
  bool res = true;
  TestTionBleLtProtocol p;
  ApiLtTest tlt(&p);

  wr_data_.clear();
  tlt.api.request_state();
  test_check(res, wr_data_, from_hex("80.0C.00.3A.AD.32.12.01.00.00.00.6C.43"));

  wr_data_.clear();
  tlt.api.request_dev_status();
  test_check(res, wr_data_, from_hex("80.0C.00.3A.AD.09.40.01.00.00.00.D1.DC"));

  tionlt_state_t st{};
  st.flags.power_state = 0;
  st.counters.work_time = 1;

  wr_data_.clear();
  tlt.api.write_state(st, 1);
  test_check(
      res, wr_data_,
      from_hex("00.1E.00.3A.AD.30.12.01.00.00.00.01.00.00.00.00.00.00.00.00.C0.00.00.00.00.00.00.00.00.00.F8.B7"));

  return res;
}
