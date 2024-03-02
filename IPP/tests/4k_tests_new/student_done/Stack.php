<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

class Stack
{
    private $stackItems;

    public function __construct()
    {
        $this->stackItems = [];
    }

    public function stackPush($item)
    {
        $this->stackItems[] = $item;
    }

    public function stackPop()
    {
        if (count($this->stackItems) === 0)
        {
            exit(56);
        }
        return array_pop($this->stackItems);
    }
}
