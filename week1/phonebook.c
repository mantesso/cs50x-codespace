#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("Enter the name: ");
    string phone = get_string("What's the phone number? ");
    int age = get_int("Enter the age ");

    printf("name is %s\n", name);
    printf("phone is %s\n", phone);
    printf("age is %i\n", age);
}