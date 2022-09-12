#include "../Core/Subsystem.h"
#include "TestCommon.h"
#include <memory>
#include <typeinfo>
#include <iostream>

class SubsystemTestFix : public ::testing::Test
{
public:
	Miyadaiku::SubsystemLocator locator;

	virtual void SetUp() override
	{
		Test::SetUp();
	}
	virtual void TearDown() override
	{
		Test::TearDown();
	}
};

TEST_F(SubsystemTestFix, AccessToSubsystem)
{
	class DummySubsystem : public Miyadaiku::Subsystem
	{
	public:
		int GetNum()
		{
			return num;
		}
		void SetNum(int _num)
		{
			num = _num;
		}

	private:
		int num = 0;
	};

	// 未作成であればnulllptrを返す
	{
		std::shared_ptr<DummySubsystem> spSubsystem =
			locator.Get<DummySubsystem>();
		ASSERT_TRUE(spSubsystem == nullptr);
	}

	locator.Add<DummySubsystem>();

	// Subsystemを取得
	{
		std::shared_ptr<DummySubsystem> spSubsystem =
			locator.Get<DummySubsystem>();
		ASSERT_TRUE(spSubsystem != nullptr);

		// Subsystemの関数にアクセス
		spSubsystem->SetNum(100);
		ASSERT_EQ(100, spSubsystem->GetNum());
	}
	// Remove
	{
		locator.Remove<DummySubsystem>();
		ASSERT_TRUE(locator.Get<DummySubsystem>() == nullptr);
	}
}
