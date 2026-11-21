DSA01 – Assignment 5: Reliable Software with Runtime Polymorphism and Exceptions

Student Details

Name: Mudireddy Charitha

Register Number: 192424118

Course: Object Oriented Programming with C++ (DSA01)

Course Outcome: CO5

Assignment: Assignment 5 – Reliable Software with Runtime Polymorphism and Exceptions

Programming Language: C++

C++ Standard: C++17

1. Assignment Overview

This assignment focuses on developing reliable and extensible C++ applications using runtime polymorphism and exception-handling mechanisms.

The main concepts covered are:

Virtual functions and pure virtual functions

Abstract classes and runtime polymorphism

Virtual tables and runtime binding

Polymorphic class hierarchies

Exception handling using try, throw and catch

Custom exception classes

Catching and rethrowing exceptions

Stack unwinding and resource safety

Smart pointers and memory management

Performance measurement and engineering trade-offs

Designing extensible and reliable software systems

The assignment uses a Hospital Billing and Insurance Claims System as its case study.

2. Development Environment

The assignment was developed using the following software and tools:

IDE: Embarcadero Dev-C++ 6.3

Compiler: TDM-GCC 9.2.0 (64-bit)

Programming Language: C++

C++ Standard: C++17

Compiler Flags: -std=c++17 -Wall -Wextra

Compilation Command

g++ -std=c++17 -Wall -Wextra filename.cpp -o program

Address Sanitizer Command

g++ -std=c++17 -Wall -Wextra -g -O1 -fsanitize=address -fno-omit-frame-pointer filename.cpp -o program

Performance Measurement Command

g++ -std=c++17 -O2 -Wall -Wextra filename.cpp -o program

Steps to Compile and Run

Open the required task folder.

Open the .cpp file in Embarcadero Dev-C++.

Select Execute → Compile & Run.

Check the program output.

Capture the execution screenshot.

Save the screenshot in the corresponding task folder.

3. Repository Structure

DSA01-192424118-Assignment5/
│
├── README.md
│
├── Task_A1/
│   ├── BillHierarchy.cpp
│   ├── UML_Diagram/
│   └── Report/
│
├── Task_A2/
│   ├── VirtualTable.cpp
│   ├── PredictOutput.cpp
│   ├── Report/
│   └── Screenshots/
│
├── Task_A3/
│   ├── PayerHierarchy.cpp
│   ├── Report/
│   └── Screenshots/
│
├── Task_A4/
│   ├── ExceptionStrategy.cpp
│   ├── ExceptionHierarchy/
│   └── Report/
│
├── Task_A5/
│   ├── ExceptionFlow.cpp
│   ├── StackUnwinding.cpp
│   └── Report/
│
├── Task_B1/
│   ├── B1_PolymorphicBilling.cpp
│   ├── TestInputs/
│   ├── Report/
│   └── Screenshots/
│
├── Task_B2/
│   ├── B2_ErrorConditions.cpp
│   ├── TestInputs/
│   ├── Report/
│   └── Screenshots/
│
├── Task_B3/
│   ├── B3_PerformanceComparison.cpp
│   ├── TestInputs/
│   ├── TimingResults/
│   ├── Report/
│   └── Screenshots/
│
└── Task_B4/
    ├── B4_FinalDecision.cpp
    ├── SanitizerResults/
    ├── Report/
    └── Screenshots/

4. Task-wise Description and Results

PART A – DESIGN

Task A1 – Design the Polymorphic Bill Hierarchy

Topic: Abstract Bill Class and Runtime Polymorphism

Identified the common operations and data shared by different bill types.

Designed an abstract Bill base class with pure virtual functions.

Designed derived classes for different hospital bill types.

Represented the hierarchy using a UML class diagram.

Explained how bills can be processed through base-class pointers.

Result: A reusable polymorphic bill hierarchy was designed to support different bill types through a common interface.

Task A2 – Understand Runtime Binding

Topic: Early Binding, Late Binding and Virtual Tables

Compared compile-time binding and runtime binding.

Explained how virtual functions support runtime polymorphism.

Designed vtable representations for the base and derived bill classes.

Examined the role of virtual destructors.

Predicted and verified the output of the given polymorphic program.

Result: Runtime function selection and the importance of virtual destructors were demonstrated.

Task A3 – Design with Abstract Classes

Topic: Payer Interface and Extensibility

Designed an abstract Payer class with a pure virtual amountCovered() function.

Designed SelfPay, Insurance and GovernmentScheme derived classes.

Considered insurance coverage limits and co-payment percentages.

Examined how a new payer can be added without modifying existing classes.

Compared abstract classes with function pointers and std::function.

Result: An extensible payer hierarchy was designed to support different payment policies.

Task A4 – Design the Exception Strategy

Topic: Error Identification and Exception Specifications

Identified possible billing errors such as negative charges and claims exceeding coverage.

Designed custom exception classes derived from standard exception classes.

Identified functions that may throw exceptions.

Considered the use of noexcept for functions that must not throw.

Explained why destructors should not throw exceptions.

Compared exception handling with return-code-based error reporting.

Result: An exception strategy was designed to handle billing errors safely and consistently.

Task A5 – Exception Flow and Resource Safety

Topic: Throwing, Catching, Rethrowing and Stack Unwinding

Examined how exceptions are thrown and caught.

Explained how stack unwinding works when an exception occurs.

Analysed the role of resource cleanup during exception handling.

Considered catch-and-rethrow behaviour.

Examined how resource safety can be maintained when errors occur.

Result: Exception flow and resource management were analysed to improve program reliability.

PART B – IMPLEMENTATION AND EXPERIMENTAL INVESTIGATION

Task B1 – Implement and Validate the Polymorphic Billing

Implemented the bill hierarchy and payer hierarchy.

Tested different bill types with different payers.

Verified bill totals using manual calculations.

Compared the implementation with the original design.

Demonstrated runtime binding by removing virtual.

Examined the effect of removing the virtual destructor using AddressSanitizer.

Result: A working polymorphic billing system was implemented and validated.

Task B2 – Engineer Error Conditions

Implemented custom exception classes.

Tested negative charges and claims exceeding coverage.

Tested unknown package codes and invalid input.

Demonstrated memory allocation failure handling.

Used try, throw and catch to handle errors.

Demonstrated exception propagation and rethrowing.

Compared exceptions with std::optional-based error handling.

Result: Different error conditions were deliberately tested and handled using exception-handling mechanisms.

Task B3 – Virtual Functions vs Switch vs Exceptions: The Cost

Compared virtual-function-based billing with a switch-based implementation.

Verified that both approaches produce matching totals for the same bills.

Measured execution time for 1,000, 1,000,000 and 10,000,000 bills.

Compared return-code and exception-based validation.

Tested validation with 0%, 1% and 50% invalid bills.

Considered std::variant and std::visit as an alternative.

Analysed performance, memory usage and engineering trade-offs.

Result: Different implementation approaches were evaluated using correctness checks and performance measurements.

Task B4 – Final Engineering Decision

Added a new MaternityBill class without modifying existing bill classes.

Added a new CharityPayer class without modifying existing payer classes.

Verified the original bill and payer combinations alongside the new extension.

Tested exception handling and resource cleanup.

Prepared the AddressSanitizer verification.

Evaluated whether polymorphism and exceptions are suitable for a hospital group.

Compared class-based extensions with configuration-based plug-ins.

Result: The billing system was extended with new bill and payer types, and its suitability for a larger hospital network was evaluated.

5. Execution and Verification

The C++ programs are compiled using the C++17 standard with the following compiler flags:

-std=c++17 -Wall -Wextra

The following verification activities are included wherever applicable:

Compilation and execution of C++ source files.

Testing of different bill types and payer combinations.

Verification of expected and actual outputs.

Runtime polymorphism and virtual-function demonstrations.

Exception throwing, catching and rethrowing.

Stack unwinding and resource cleanup analysis.

AddressSanitizer testing.

Performance measurements using optimized compilation.

Comparison of alternative implementation approaches.

Testing of new bill and payer extensions.

Execution screenshots, test inputs, timing results and sanitizer reports are stored in their corresponding task folders.

6. Learning Outcomes

Through this assignment, I gained practical understanding of:

Designing abstract classes and reusable class hierarchies.

Implementing virtual and pure virtual functions.

Understanding early binding and late binding.

Applying runtime polymorphism using base-class pointers.

Understanding virtual tables and virtual destructors.

Designing extensible payer and billing systems.

Creating and handling custom exception classes.

Using throw, try, catch and rethrowing.

Understanding stack unwinding and resource safety.

Comparing exceptions with alternative error-handling mechanisms.

Measuring execution performance and memory usage.

Evaluating software designs based on reliability, extensibility and efficiency.

7. Conclusion

This assignment provided practical experience in developing reliable and extensible C++ applications using runtime polymorphism and exception handling.

The hospital billing system demonstrated how abstract classes, virtual functions and inheritance support reusable designs, while exception handling and resource management help maintain program safety when errors occur.

The implementation and experimental investigations also helped evaluate different design approaches based on correctness, performance, memory safety and extensibility.

All task-wise source files, reports, test inputs and execution evidence are organised in their respective folders for verification.
