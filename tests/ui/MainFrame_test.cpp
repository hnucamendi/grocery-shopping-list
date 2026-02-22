#include "ui/MainFrame.h"
#include <catch2/catch_test_macros.hpp>
#include <wx/init.h>

TEST_CASE("Test MainFrame attributes") {
    wxInitialize();
    {
        const MainFrame frame;
        REQUIRE(frame.GetTitle() == "Grocery Shopping List");
    }
    wxUninitialize();
}
