// oop-demo.ts

// Abstract base class showing shared behavior and abstraction.
abstract class Person {
    static schoolName = 'Modern Learning Academy';
    readonly id: number;
    protected firstName: string;
    protected lastName: string;

    constructor(id: number, firstName: string, lastName: string) {
        this.id = id;
        this.firstName = firstName;
        this.lastName = lastName;
    }

    get fullName(): string {
        return `${this.firstName} ${this.lastName}`;
    }

    abstract getRoleDescription(): string;
}

// Interface declaring a contract for paid staff.
interface Payable {
    calculatePay(): number;
}

// Student class illustrates inheritance and encapsulation.
class Student extends Person {
    private department: string;
    private grade: number;

    constructor(id: number, firstName: string, lastName: string, department: string, grade: number) {
        super(id, firstName, lastName);
        this.department = department;
        this.grade = grade;
    }

    getRoleDescription(): string {
        return `Student in ${this.department} with grade ${this.grade}`;
    }
}

// Teacher class shows inheritance, overriding, and protected properties.
class Teacher extends Person implements Payable {
    private subject: string;
    private salaryPerMonth: number;

    constructor(id: number, firstName: string, lastName: string, subject: string, salaryPerMonth: number) {
        super(id, firstName, lastName);
        this.subject = subject;
        this.salaryPerMonth = salaryPerMonth;
    }

    getRoleDescription(): string {
        return `Teacher for ${this.subject}`;
    }

    calculatePay(): number {
        return this.salaryPerMonth;
    }
}

// Polymorphic helper that works with any Person subclass.
function printPersonInfo(person: Person): string {
    return `ID: ${person.id} | Name: ${person.fullName} | Role: ${person.getRoleDescription()}`;
}

function runDemo(): void {
    const people: Person[] = [
        new Student(1, 'Alicia', 'Brown', 'Computer Science', 92),
        new Teacher(2, 'Marcus', 'Reed', 'Web Development', 4200),
    ];

    const outputLines = [
        `School: ${Person.schoolName}`,
        '--- Object-Oriented Programming Demo ---',
    ];

    people.forEach(person => {
        outputLines.push(printPersonInfo(person));
        if (person instanceof Teacher) {
            outputLines.push(`Monthly pay: $${person.calculatePay()}`);
        }
    });

    const result = outputLines.join('\n');

    if (typeof document !== 'undefined') {
        const resultElement = document.getElementById('result');
        if (resultElement) {
            resultElement.textContent = result;
            return;
        }
    }

    console.log(result);
}

runDemo();
