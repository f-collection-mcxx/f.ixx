import f;
import std;

class PolymorphicBase {
public:
    virtual ~PolymorphicBase() = default;
    virtual int value() const { return 0; }
};

class Derived1 : public PolymorphicBase {
public:
    int v;
    Derived1(int val) : v(val) {}
    int value() const override { return v; }
};

class Derived2 : public PolymorphicBase {
public:
    std::string name;
    Derived2(std::string n) : name(std::move(n)) {}
    int value() const override { return 42; }
};

int main() {
    std::pmr::memory_resource* resource = std::pmr::new_delete_resource();

    // Test 1: Create dynamic_deque and emplace_back
    {
        f::dynamic_deque<PolymorphicBase> dq{resource};
        Derived1& d1 = dq.emplace_back<Derived1>(10);
        Derived1& d2 = dq.emplace_back<Derived1>(20);

        if (dq.size() == 2 && d1.value() == 10 && d2.value() == 20) {
            std::println("✓ Test 1 passed: emplace_back works correctly");
        } else {
            std::println("✗ Test 1 failed: emplace_back issue");
            return 1;
        }
    }

    // Test 2: emplace_front
    {
        f::dynamic_deque<PolymorphicBase> dq{resource};
        Derived1& d1 = dq.emplace_front<Derived1>(100);
        Derived1& d2 = dq.emplace_front<Derived1>(200);

        if (dq.size() == 2 && dq.front()->value() == 200 && dq.back()->value() == 100) {
            std::println("✓ Test 2 passed: emplace_front works correctly");
        } else {
            std::println("✗ Test 2 failed: emplace_front issue");
            return 1;
        }
    }

    // Test 3: release_back
    {
        f::dynamic_deque<PolymorphicBase> dq{resource};
        dq.emplace_back<Derived1>(30);
        dq.emplace_back<Derived1>(40);

        auto ptr = dq.release_back();
        if (dq.size() == 1 && ptr->value() == 40) {
            std::println("✓ Test 3 passed: release_back works correctly");
        } else {
            std::println("✗ Test 3 failed: release_back issue");
            return 1;
        }
    }

    // Test 4: release_front
    {
        f::dynamic_deque<PolymorphicBase> dq{resource};
        dq.emplace_back<Derived1>(50);
        dq.emplace_back<Derived1>(60);

        auto ptr = dq.release_front();
        if (dq.size() == 1 && ptr->value() == 50) {
            std::println("✓ Test 4 passed: release_front works correctly");
        } else {
            std::println("✗ Test 4 failed: release_front issue");
            return 1;
        }
    }

    // Test 5: clear
    {
        f::dynamic_deque<PolymorphicBase> dq{resource};
        dq.emplace_back<Derived1>(70);
        dq.emplace_back<Derived1>(80);
        dq.clear();

        if (dq.empty() && dq.size() == 0) {
            std::println("✓ Test 5 passed: clear works correctly");
        } else {
            std::println("✗ Test 5 failed: clear issue");
            return 1;
        }
    }

    // Test 6: swap
    {
        f::dynamic_deque<PolymorphicBase> dq1{resource};
        f::dynamic_deque<PolymorphicBase> dq2{resource};
        dq1.emplace_back<Derived1>(90);
        dq2.emplace_back<Derived1>(100);

        dq1.swap(dq2);
        if (dq1.back()->value() == 100 && dq2.back()->value() == 90) {
            std::println("✓ Test 6 passed: swap works correctly");
        } else {
            std::println("✗ Test 6 failed: swap issue");
            return 1;
        }
    }

    // Test 7: move constructor
    {
        f::dynamic_deque<PolymorphicBase> dq1{resource};
        dq1.emplace_back<Derived1>(110);
        f::dynamic_deque<PolymorphicBase> dq2{std::move(dq1)};

        if (dq2.size() == 1 && dq2.back()->value() == 110) {
            std::println("✓ Test 7 passed: move constructor works");
        } else {
            std::println("✗ Test 7 failed: move constructor issue");
            return 1;
        }
    }

    // Test 8: Different derived types
    {
        f::dynamic_deque<PolymorphicBase> dq{resource};
        Derived1& d1 = dq.emplace_back<Derived1>(120);
        Derived2& d2 = dq.emplace_back<Derived2>("test");

        if (dq.size() == 2 && d1.value() == 120 && d2.value() == 42) {
            std::println("✓ Test 8 passed: different derived types work");
        } else {
            std::println("✗ Test 8 failed: derived types issue");
            return 1;
        }
    }

    std::println("\n✓ All deque tests passed!");
    return 0;
}
