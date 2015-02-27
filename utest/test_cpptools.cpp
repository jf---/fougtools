#include "test_cpptools.h"

#include "../src/cpptools/basic_shared_pointer.h"
#include "../src/cpptools/c_array_utils.h"
#include "../src/cpptools/circular_iterator.h"
#include "../src/cpptools/hash_fnv.h"
#include "../src/cpptools/memory_utils.h"
#include "../src/cpptools/pusher.h"
#include "../src/cpptools/quantity.h"
#include "../src/cpptools/scoped_value.h"

#include <QtCore/QScopedPointer>
#include <QtCore/QtDebug>

#include <queue>

// --
// -- Functor<> tests
// --

class Foo
{
public:
    virtual int bar()
    { return 1; }

    int constBar() const
    { return 2; }

    int bar1(double v)
    { return qRound(v); }

    int constBar1(double v) const
    { return qRound(v) + 1; }

    int bar2(float v, double w)
    { return qRound(v + w); }

    int constBar2(float v, double w) const
    { return qRound(v + w) + 1; }
};

class FooHeir : public Foo
{
public:
    int bar()
    { return 3; }
};

int func0()
{ return 4; }

int func1(double v)
{ return qRound(v) * 2; }

int func2(float v, double w)
{ return qRound(v + w) * 2; }

struct DeleteHook
{
    DeleteHook(int valOnDelete, int* spyInt)
        : m_spyInt(spyInt),
          m_valOnDelete(valOnDelete)
    { }

    ~DeleteHook()
    { *m_spyInt = m_valOnDelete; }

private:
    int* m_spyInt;
    const int m_valOnDelete;
};

void TestCppTools::BasicSharedPointer_test()
{
    int spyHint = 0;
    {
        DeleteHook* delHook = new DeleteHook(-1, &spyHint);
        cpp::BasicSharedPointer<DeleteHook> sharedPtr(delHook);
    }
    QCOMPARE(spyHint, -1);

    spyHint = 0;
    {
        DeleteHook* delHook = new DeleteHook(-1, &spyHint);
        cpp::BasicSharedPointer<DeleteHook> sharedPtr(delHook);
        QCOMPARE(sharedPtr.data(), delHook);

        cpp::BasicSharedPointer<DeleteHook> sharedPtr1;
        QVERIFY(sharedPtr1.isNull());
        sharedPtr1 = sharedPtr;
        QCOMPARE(sharedPtr1.data(), delHook);
    }
    QCOMPARE(spyHint, -1);
}

void TestCppTools::cArrayUtils_test()
{
    int array1[1];
    int array5[5];
    QCOMPARE(cpp::cArraySize(array1), static_cast<size_t>(1));
    QCOMPARE(cpp::cArraySize(array5), static_cast<size_t>(5));

    QCOMPARE(cpp::cArrayEnd(array1), array1 + 1);
    QCOMPARE(cpp::cArrayEnd(array5), array5 + 5);
}

void TestCppTools::ScopedValue_test()
{
    bool b = true;
    {
        cpp::ScopedBool sb(b, true); Q_UNUSED(sb);
        QCOMPARE(b, true);
    }
    QCOMPARE(b, true);
    b = true;
    {
        cpp::ScopedBool sb(b, false); Q_UNUSED(sb);
        QCOMPARE(b, false);
    }
    QCOMPARE(b, true);

    b = false;
    {
        cpp::ScopedBool sb(b, true); Q_UNUSED(sb);
        QCOMPARE(b, true);
    }
    QCOMPARE(b, false);
    b = false;
    {
        cpp::ScopedBool sb(b, false); Q_UNUSED(sb);
        QCOMPARE(b, false);
    }
    QCOMPARE(b, false);
}

void TestCppTools::circularIterator_test()
{
    QCOMPARE(cpp::circularNext(0, 5, 1), 2);
    QCOMPARE(cpp::circularNext(0, 5, 0), 1);
    QCOMPARE(cpp::circularNext(0, 5, 4), 0);
    QCOMPARE(cpp::circularNext(0, 1, 0), 0);

    QCOMPARE(cpp::circularPrior(0, 5, 1), 0);
    QCOMPARE(cpp::circularPrior(0, 5, 0), 4);
    QCOMPARE(cpp::circularPrior(0, 5, 4), 3);
    QCOMPARE(cpp::circularPrior(0, 1, 0), 0);

    QCOMPARE(cpp::circularAdvance(0, 5, 1, -1), cpp::circularPrior(0, 5, 1));
    QCOMPARE(cpp::circularAdvance(0, 5, 0, -1), cpp::circularPrior(0, 5, 0));
    QCOMPARE(cpp::circularAdvance(0, 5, 4, -1), cpp::circularPrior(0, 5, 4));
    QCOMPARE(cpp::circularAdvance(0, 1, 0, -1), cpp::circularPrior(0, 1, 0));

    QCOMPARE(cpp::circularAdvance(0, 5, 1, 1), cpp::circularNext(0, 5, 1));
    QCOMPARE(cpp::circularAdvance(0, 5, 0, 1), cpp::circularNext(0, 5, 0));
    QCOMPARE(cpp::circularAdvance(0, 5, 4, 1), cpp::circularNext(0, 5, 4));
    QCOMPARE(cpp::circularAdvance(0, 1, 0, 1), cpp::circularNext(0, 1, 0));

    QCOMPARE(cpp::circularAdvance(0, 5, 0, 3), 3);
    QCOMPARE(cpp::circularAdvance(0, 5, 0, -3), 2);
    QCOMPARE(cpp::circularAdvance(0, 5, 0, 6), 1);
}

namespace memoryUtils_test {

struct DummyObserver
{
    bool isDummyDeleted;
}; // struct DummyObserver

struct Dummy
{
    Dummy(DummyObserver* obs) :
        m_a(0),
        m_b(0),
        m_observer(obs)
    {
        obs->isDummyDeleted = false;
    }

    ~Dummy()
    {
        m_observer->isDummyDeleted = true;
    }

    int m_a;
    double m_b;
    DummyObserver* m_observer;
};

} // namespace memoryUtils_test

void TestCppTools::memoryUtils_test()
{
    memoryUtils_test::DummyObserver observer;
    memoryUtils_test::Dummy* dummy = new memoryUtils_test::Dummy(&observer);
    const bool oldIsDeletedTag = observer.isDummyDeleted;
    cpp::checkedReset(dummy);
    QVERIFY(!oldIsDeletedTag);
    QVERIFY(dummy == NULL);
    QVERIFY(observer.isDummyDeleted);
}

void TestCppTools::pusher_test()
{
    std::queue<int> intq;
    auto it = cpp::pusher(intq);
    *it = 5; ++it;
    *it = 10; ++it;

    QCOMPARE(intq.front(), 5);
    intq.pop();
    QCOMPARE(intq.front(), 10);
}

namespace hash_fnv_test {

struct TestData
{
    const char* byteSeq;
    std::uint32_t hash32_fnv_1a;
    std::uint64_t hash64_fnv_1a;
};

} // namespace hash_fnv_test

void TestCppTools::hash_fnv_test()
{
    // TODO: fill testDataVec with http://www.isthe.com/chongo/src/fnv/test_fnv.c
    const hash_fnv_test::TestData testDataVec[] =
    {
        { "", 0x811c9dc5UL, 0xcbf29ce484222325ULL },
        { "a", 0xe40c292cUL, 0xaf63dc4c8601ec8cULL },
        { "b", 0xe70c2de5UL, 0xaf63df4c8601f1a5ULL },
        { "c", 0xe60c2c52UL, 0xaf63de4c8601eff2ULL },
        { "d", 0xe10c2473UL, 0xaf63d94c8601e773ULL },
        { "e", 0xe00c22e0UL, 0xaf63d84c8601e5c0ULL }
    };
    for (const auto& testData : testDataVec) {
        cpp::hash32_fnv_1a hash32;
        cpp::hash64_fnv_1a hash64;
        const auto byteSeqLen = std::strlen(testData.byteSeq);

        QCOMPARE(hash32(testData.byteSeq), testData.hash32_fnv_1a);
        QCOMPARE(hash32(testData.byteSeq, byteSeqLen), testData.hash32_fnv_1a);
        QCOMPARE(hash32(testData.byteSeq, testData.byteSeq + byteSeqLen), testData.hash32_fnv_1a);

        QCOMPARE(hash64(testData.byteSeq), testData.hash64_fnv_1a);
        QCOMPARE(hash64(testData.byteSeq, byteSeqLen), testData.hash64_fnv_1a);
        QCOMPARE(hash64(testData.byteSeq, testData.byteSeq + byteSeqLen), testData.hash64_fnv_1a);
    }
}

// --
// -- Quantity tests
// --

struct LengthTrait {};
typedef cpp::Quantity<cpp::DoubleNumericTraits, LengthTrait> Length;
typedef cpp::Quantity<cpp::IntNumericTraits, LengthTrait> Length_i;

struct AngleTrait {};
typedef cpp::Quantity<cpp::DoubleNumericTraits, AngleTrait> Angle;

void TestCppTools::Quantity_test()
{
    Length length1(1.);
    Length length2(length1 * 5);
    Length length3;
    length3 = length2;
    //qDebug() << length1 << length2;

    QCOMPARE(sizeof(Length), sizeof(double));
}

void TestCppTools::Quantity_operators_test()
{
    QCOMPARE((Length_i(5) + Length_i(10)).value(), 15);
    QCOMPARE((Length_i(5) - Length_i(10)).value(), -5);
    QCOMPARE((Length_i(5) * Length_i(10)).value(), 50);
    QCOMPARE((Length_i(10) / Length_i(5)).value(), 2);

    QCOMPARE((2 * Length_i(10)).value(), 20);
    QCOMPARE((Length_i(15) * 2).value(), 30);
    QCOMPARE((2 * Length_i(15) * 2).value(), 60);
    QCOMPARE((2 + Length_i(15)).value(), 17);
    QCOMPARE((2 - Length_i(15)).value(), 13);
}

void TestCppTools::Quantity_benchmark1()
{
    //  QBENCHMARK {
    //    Length length;
    //    for (int i = 0; i < 100000000; ++i)
    //      length = Length(i + length);
    //    qDebug() << length.value();
    //  }
}

void TestCppTools::Quantity_benchmark2()
{
    //  QBENCHMARK {
    //    double length = 0;
    //    for (int i = 0; i < 100000000; ++i)
    //      length = i + length;
    //    qDebug() << length;
    //  }
}
