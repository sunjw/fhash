#include "stdafx.h"
#include "TestDelegate.h"

using namespace Platform;
using namespace FilesHashUwp;

TestDelegate::TestDelegate()
{

}

void TestDelegate::OnGetHello(String^ strHello)
{
	OnHelloHandler(strHello);
}
