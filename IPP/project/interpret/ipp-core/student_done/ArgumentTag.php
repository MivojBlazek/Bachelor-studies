<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

class ArgumentTag
{
    private $type;
    private $value;

    public function __construct($type, $value)
    {
        $this->type = $type;
        $this->value = $value;
    }

    // method returns type of argument
    public function getType()
    {
        return $this->type;
    }

    // method returns value of argument
    public function getValue()
    {
        return $this->value;
    }
}
