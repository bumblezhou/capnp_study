@0x85150b117366d14a;

interface TestInterface {
    add @0 (params :List(Float64)) -> (value :Float64);
    split @1 (param :Text) -> (result :List(Text));
}