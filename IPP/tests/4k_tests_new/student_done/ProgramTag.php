<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

use IPP\Student\InstructionTag;
use IPP\Student\Frame;
use IPP\Student\Stack;
use IPP\Core\FileInputReader;

class ProgramTag
{
    private $language; //TODO
    private $name; //TODO
    private $description; //TODO
    private $instructions = [];
    private $labels = [];

    public function __construct($language, $name = null, $description = null)
    {
        $this->language = $language; //TODO
        $this->name = $name ?? ''; //TODO
        $this->description = $description ?? ''; //TODO
    }

    // method adds instruction to array
    public function addInstruction(InstructionTag $instr)
    {
        $this->instructions[] = $instr;
    }

    // method returns instruction array
    public function getInstructions()
    {
        return $this->instructions;
    }

    // method sorts instructions in array
    public function sortInstructions()
    {
        usort($this->instructions, array($this, 'sortOrders'));
    }

    // helper method for sorting
    private function sortOrders($first, $second)
    {
        if ($first->getOrder() < $second->getOrder())
        {
            return -1;
        }
        elseif ($first->getOrder() > $second->getOrder())
        {
            return 1;
        }
        else
        {
            throw new \Exception("Error Processing Request", 32);
        }
    }

    // method executes program by executing instructions
    public function executeProgram($stdout, $stderr, $inputReader)
    {
        $frames = new Frame();
        $stack = new Stack();
        $functionStack = new Stack();
        $this->findLabels();
        
        $instrIndex = 0;
        $instrCount = count($this->instructions);
        while ($instrIndex < $instrCount)
        {
            $instruction = $this->instructions[$instrIndex];
            $jumpToLabel = $instruction->executeInstr($instrIndex, $this->labels, $frames, $stack, $functionStack, $stdout, $stderr, $inputReader);
            
            if (empty($jumpToLabel))
            {
                $instrIndex++;
            }
            elseif (substr($jumpToLabel, 0, 6) === '#CALL#')
            {
                $instrIndex = substr($jumpToLabel, 6, strlen($jumpToLabel));
                $instrIndex++;
            }
            else
            {
                $instrIndex = $this->labels[$jumpToLabel];
            }
        }
    }

    // method finds instructions LABEL and saves their position
    private function findLabels()
    {
        foreach ($this->instructions as $labelIndex => $instruction)
        {
            if ($instruction->getOpcode() === 'LABEL')
            {
                $labelName = $instruction->getArguments()[0]->getValue();
                if (isset($this->labels[$labelName]))
                {
                    exit(52);
                }
                $this->labels[$labelName] = $labelIndex;
            }
        }
    }
}
