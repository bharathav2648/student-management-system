class student
{
    name:string;
    dept:string;
    constructor(name1:string,dept1:string)
    {
        this.name=name1;
        this.dept=dept1;
    }
    display(): string
    {
       return "Name:" +this.name+ "Dept:" +this.dept;
    }
}
function display1(): void
{
   let stud=new student("ace","cse");
   (document.getElementById("result") as HTMLElement).innerHTML=stud.display();
}