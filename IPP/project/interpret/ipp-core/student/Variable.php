<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

class Variable
{
    private string $type;
    public string $name;
    private string $value;

    public function __construct(string $name)
    {
        $this->type = 'nil';
        $this->name = $name;
    }

    // method sets variable value and type
    public function setValue(string $value): void
    {
        $this->value = $value;
        if (is_numeric($this->value) && intval($this->value) == $this->value)
        {
            $this->type = 'int';
        }
        elseif ($this->value === 'true' || $this->value === 'false')
        {
            $this->type = 'bool';
        }
        elseif ($this->value === 'nil')
        {
            $this->type = 'nil';
        }
        else
        {
            $this->type = 'string';
        }
    }

    // method returns name of variable
    public function getName(): string
    {
        return $this->name;
    }

    // method returns value of variable
    public function getValue(): string
    {

        if (!isset($this->value))
        {
            exit(56);
        }
        if ($this->value === 'nil')
        {
            exit(56);
        }
        return $this->value;
    }

    // method returns value of variable with nil included
    public function getValueNil(): string
    {
        if (!isset($this->value))
        {
            exit(56);
        }
        return $this->value;
    }

    // method returns type of variable
    public function getType(): string
    {
        if (is_numeric($this->value) && intval($this->value) == $this->value)
        {
            $this->type = 'int';
        }
        elseif ($this->value === 'true' || $this->value === 'false')
        {
            $this->type = 'bool';
        }
        elseif ($this->value === 'nil')
        {
            $this->type = 'nil';
        }
        else
        {
            $this->type = 'string';
        }
        return $this->type;
    }
}
