// RUN: %clang_cc1 -cxx-abi itanium -emit-llvm %s -o - | FileCheck %s
namespace foo {

// CHECK-NOT: @a = global i32
extern "C" int a;

// CHECK-NOT: @_ZN3foo1bE = global i32
extern int b;

// CHECK: @_ZN3foo1cE = global i32
int c = 5;

// CHECK-NOT: @_ZN3foo1dE
extern "C" struct d;

}

namespace test1 {
  namespace {
    struct X {};
  }
  extern "C" {
    // CHECK: @test1_b = global
    X test1_b = X();
  }
  void *use = &test1_b;
  // CHECK: @_ZN5test13useE = global
}

namespace test2 {
  namespace {
    struct X {};
  }

  // CHECK: @test2_b = global
  extern "C" X test2_b;
  X test2_b;
}

extern "C" {
  static int unused_var;
  static int unused_fn() { return 0; }

  __attribute__((used)) static int internal_var;
  __attribute__((used)) static int internal_fn() { return 0; }

  __attribute__((used)) static int duplicate_internal_var;
  __attribute__((used)) static int duplicate_internal_fn() { return 0; }

  namespace N {
    __attribute__((used)) static int duplicate_internal_var;
    __attribute__((used)) static int duplicate_internal_fn() { return 0; }
  }

  // CHECK: @llvm.used = appending global {{.*}} @internal_var {{.*}} @internal_fn 

  // CHECK-NOT: @unused
  // CHECK-NOT: @duplicate_internal
  // CHECK: @internal_var = alias internal i32* @_Z12internal_var
  // CHECK-NOT: @unused
  // CHECK-NOT: @duplicate_internal
  // CHECK: @internal_fn = alias internal i32 ()* @_Z11internal_fnv
  // CHECK-NOT: @unused
  // CHECK-NOT: @duplicate_internal
}
