<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

class Stack
{
    /** @var string[] */
    private $stackItems;

    public function __construct()
    {
        $this->stackItems = [];
    }

    public function stackPush(string $item): void
    {
        $this->stackItems[] = $item;
    }

    public function stackPop(): string
    {
        if (count($this->stackItems) === 0)
        {
            exit(56);
        }
        return array_pop($this->stackItems);
    }
}
