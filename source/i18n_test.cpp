#include "unit_test.h"
#include "i18n.h"

TEST(ZI18NTest)
{
    I18N i18n("text\\test");
    TinyString str = i18n.getString(100);
    ASSERT_TRUE(str == "This is yet another sample text.");

    str = i18n.getString(1);
    ASSERT_TRUE(str == "\xE4\xF6\xFC\xDF");

    str = i18n.getString(123);
    ASSERT_TRUE(str == "???");
}
