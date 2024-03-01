use crate_and_module_sandbox::test_lib_1;
use crate_and_module_sandbox::test_lib_oop::StringPrinter;

fn main() {
    test_lib_1::helloworld::print_hello_world();
    test_lib_1::print_lots_of_crap::print_lots_of_crap();

    let mut string_printer_instance = StringPrinter::new();

    string_printer_instance.print_string();
    let new_string: String = String::from("hellooooooo!");
    string_printer_instance.set_string(new_string);

    string_printer_instance.print_string();
}
