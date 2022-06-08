#include "pch.h"
#include "TestDelegate.h"

using namespace Platform;
using namespace fHashUwp;

TestDelegate::TestDelegate()
{

}

void TestDelegate::OnGetHello(String^ strHello)
{
	OnHelloHandler(strHello);
}
