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

    // Test 1: Create dynamic_vector and emplace_back
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        Derived1& d1 = vec.emplace_back<Derived1>(10);
        Derived1& d2 = vec.emplace_back<Derived1>(20);

        if (vec.size() == 2 && d1.value() == 10 && d2.value() == 20) {
            std::println("✓ Test 1 passed: emplace_back works correctly");
        } else {
            std::println("✗ Test 1 failed: emplace_back issue");
            return 1;
        }
    }

    // Test 2: Access elements with operator[]
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        vec.emplace_back<Derived1>(100);
        vec.emplace_back<Derived1>(200);
        vec.emplace_back<Derived1>(300);

        if (vec[0]->value() == 100 && vec[1]->value() == 200 && vec[2]->value() == 300) {
            std::println("✓ Test 2 passed: operator[] works correctly");
        } else {
            std::println("✗ Test 2 failed: operator[] issue");
            return 1;
        }
    }

    // Test 3: Access elements with at()
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        vec.emplace_back<Derived1>(400);
        vec.emplace_back<Derived1>(500);

        if (vec.at(0)->value() == 400 && vec.at(1)->value() == 500) {
            std::println("✓ Test 3 passed: at() works correctly");
        } else {
            std::println("✗ Test 3 failed: at() issue");
            return 1;
        }
    }

    // Test 4: release_back
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        vec.emplace_back<Derived1>(600);
        vec.emplace_back<Derived1>(700);

        auto ptr = vec.release_back();
        if (vec.size() == 1 && ptr->value() == 700) {
            std::println("✓ Test 4 passed: release_back works correctly");
        } else {
            std::println("✗ Test 4 failed: release_back issue");
            return 1;
        }
    }

    // Test 5: clear
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        vec.emplace_back<Derived1>(800);
        vec.emplace_back<Derived1>(900);
        vec.clear();

        if (vec.empty() && vec.size() == 0) {
            std::println("✓ Test 5 passed: clear works correctly");
        } else {
            std::println("✗ Test 5 failed: clear issue");
            return 1;
        }
    }

    // Test 6: swap
    {
        f::dynamic_vector<PolymorphicBase> vec1{resource};
        f::dynamic_vector<PolymorphicBase> vec2{resource};
        vec1.emplace_back<Derived1>(1000);
        vec2.emplace_back<Derived1>(2000);

        vec1.swap(vec2);
        if (vec1.back()->value() == 2000 && vec2.back()->value() == 1000) {
            std::println("✓ Test 6 passed: swap works correctly");
        } else {
            std::println("✗ Test 6 failed: swap issue");
            return 1;
        }
    }

    // Test 7: move constructor
    {
        f::dynamic_vector<PolymorphicBase> vec1{resource};
        vec1.emplace_back<Derived1>(3000);
        f::dynamic_vector<PolymorphicBase> vec2{std::move(vec1)};

        if (vec2.size() == 1 && vec2.back()->value() == 3000) {
            std::println("✓ Test 7 passed: move constructor works");
        } else {
            std::println("✗ Test 7 failed: move constructor issue");
            return 1;
        }
    }

    // Test 8: move assignment
    {
        f::dynamic_vector<PolymorphicBase> vec1{resource};
        vec1.emplace_back<Derived1>(4000);
        f::dynamic_vector<PolymorphicBase> vec2{resource};
        vec2 = std::move(vec1);

        if (vec2.size() == 1 && vec2.back()->value() == 4000) {
            std::println("✓ Test 8 passed: move assignment works");
        } else {
            std::println("✗ Test 8 failed: move assignment issue");
            return 1;
        }
    }

    // Test 9: Different derived types
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        Derived1& d1 = vec.emplace_back<Derived1>(5000);
        Derived2& d2 = vec.emplace_back<Derived2>("test");

        if (vec.size() == 2 && d1.value() == 5000 && d2.value() == 42) {
            std::println("✓ Test 9 passed: different derived types work");
        } else {
            std::println("✗ Test 9 failed: derived types issue");
            return 1;
        }
    }

    // Test 10: front and back
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        vec.emplace_back<Derived1>(6000);
        vec.emplace_back<Derived1>(7000);

        if (vec.front()->value() == 6000 && vec.back()->value() == 7000) {
            std::println("✓ Test 10 passed: front and back work correctly");
        } else {
            std::println("✗ Test 10 failed: front/back issue");
            return 1;
        }
    }

    // Test 11: iteration
    {
        f::dynamic_vector<PolymorphicBase> vec{resource};
        vec.emplace_back<Derived1>(8000);
        vec.emplace_back<Derived1>(9000);
        vec.emplace_back<Derived1>(10000);

        int sum = 0;
        for (const auto& elem : vec) {
            sum += elem->value();
        }

        if (sum == 8000 + 9000 + 10000) {
            std::println("✓ Test 11 passed: iteration works correctly");
        } else {
            std::println("✗ Test 11 failed: iteration issue, got sum={}", sum);
            return 1;
        }
    }

    std::println("\n✓ All vector tests passed!");
    return 0;
}
