<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

class ArgumentTag
{
    private string $type;
    private string $value;

    public function __construct(string $type, string $value)
    {
        $this->type = $type;
        $this->value = $value;
    }

    // method returns type of argument
    public function getType(): string
    {
        return $this->type;
    }

    // method returns value of argument
    public function getValue(): string
    {
        return $this->value;
    }
}
