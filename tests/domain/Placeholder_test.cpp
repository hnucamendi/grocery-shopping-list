#include "ui/MainFrame.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Grocery Shopping List Frame loads window") {
    MainFrame frame;
  REQUIRE(frame.GetTitle() == wxString("Grocery Shopping List"));
}
