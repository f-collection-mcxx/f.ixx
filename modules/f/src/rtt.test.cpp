import f;
import std;

class Base {
public:
    virtual ~Base() = default;
    virtual int value() const { return 0; }
};

class Derived : public Base {
public:
    int v;
    Derived(int val = 0) : v(val) {}
    int value() const override { return v; }
};

int main() {
    // Test 1: type_t::of and size/align
    {
        auto type_info = f::type_t::of<int>();
        if (type_info->size() == sizeof(int) && type_info->align() == alignof(int)) {
            std::println("✓ Test 1 passed: type_t size and align");
        } else {
            std::println("✗ Test 1 failed");
            return 1;
        }
    }

    // Test 2: type_t equality
    {
        auto t1 = f::type_t::of<int>();
        auto t2 = f::type_t::of<int>();
        auto t3 = f::type_t::of<double>();

        if (t1 == t2 && t1 != t3) {
            std::println("✓ Test 2 passed: type_t equality");
        } else {
            std::println("✗ Test 2 failed");
            return 1;
        }
    }

    // Test 3: type_t make (default construct)
    {
        auto type_info = f::type_t::of<int>();
        alignas(int) std::byte buffer[sizeof(int)];
        type_info->make(buffer);
        if (*reinterpret_cast<int*>(buffer) == 0) {
            std::println("✓ Test 3 passed: type_t default construct");
        } else {
            std::println("✗ Test 3 failed");
            return 1;
        }
    }

    // Test 4: type_t make (copy construct)
    {
        auto type_info = f::type_t::of<int>();
        int proto = 42;
        alignas(int) std::byte buffer[sizeof(int)];

        type_info->make(buffer, reinterpret_cast<const std::byte*>(&proto));

        if (*reinterpret_cast<int*>(buffer) == proto) {
            std::println("✓ Test 4 passed: type_t copy construct");
        } else {
            std::println("✗ Test 4 failed");
            return 1;
        }
    }

    // Test 5: type_t destroy
    {
        auto type_info = f::type_t::of<int>();
        alignas(int) std::byte buffer[sizeof(int)];
        type_info->make(buffer);
        type_info->destroy(buffer);
        std::println("✓ Test 5 passed: type_t destroy");
    }

    // Test 6: type_t try_make
    {
        auto type_info = f::type_t::of<int>();
        alignas(int) std::byte buffer[sizeof(int)];
        if (type_info->try_make(buffer)) {
            std::println("✓ Test 6 passed: type_t try_make succeeds for default constructible");
        } else {
            std::println("✗ Test 6 failed");
            return 1;
        }
    }

    // Test 7: is_instance
    {
        Derived d{100};
        Base& b = d;

        if (f::is_instance<Derived>(b)) {
            std::println("✓ Test 7 passed: is_instance works");
        } else {
            std::println("✗ Test 7 failed");
            return 1;
        }
    }

    // Test 8: as (reference cast)
    {
        Derived d{200};
        Base& b = d;

        auto& derived_ref = f::as<Derived>(b);
        if (derived_ref.value() == 200) {
            std::println("✓ Test 8 passed: as<Derived>(base_ref) works");
        } else {
            std::println("✗ Test 8 failed");
            return 1;
        }
    }

    // Test 9: as (pointer cast)
    {
        Derived d{300};
        Base* b = &d;

        auto* derived_ptr = f::as<Derived>(b);
        if (derived_ptr && derived_ptr->value() == 300) {
            std::println("✓ Test 9 passed: as<Derived>(base_ptr) works");
        } else {
            std::println("✗ Test 9 failed");
            return 1;
        }
    }

    // Test 10: any emplace and as
    {
        f::any a;
        int& ref = a.emplace<int>(42);
        if (ref == 42 && a.as<int>() == 42) {
            std::println("✓ Test 10 passed: any emplace and as");
        } else {
            std::println("✗ Test 10 failed");
            return 1;
        }
    }

    // Test 11: any copy and move
    {
        f::any a1;
        a1.emplace<std::string>("hello");

        f::any a2{a1};
        if (a2.as<std::string>() == "hello") {
            std::println("✓ Test 11 passed: any copy constructor");
        } else {
            std::println("✗ Test 11 failed");
            return 1;
        }

        f::any a3{std::move(a2)};
        if (a3.as<std::string>() == "hello") {
            std::println("✓ Test 11 passed: any move constructor");
        } else {
            std::println("✗ Test 11 failed");
            return 1;
        }
    }

    // Test 12: any clear
    {
        f::any a;
        a.emplace<int>(100);
        a.clear();
        try {
            a.as<int>();
            std::println("✗ Test 12 failed: should throw bad_any_cast");
            return 1;
        } catch (const std::bad_any_cast&) {
            std::println("✓ Test 12 passed: any clear works");
        }
    }

    // Test 13: method_not_implemented exception
    {
        try {
            throw f::method_not_implemented{"test_method"};
        } catch (const f::method_not_implemented& e) {
            if (e.method_name == "test_method") {
                std::println("✓ Test 13 passed: method_not_implemented");
            } else {
                std::println("✗ Test 13 failed");
                return 1;
            }
        }
    }

    std::println("\n✓ All rtt tests passed!");
    return 0;
}
