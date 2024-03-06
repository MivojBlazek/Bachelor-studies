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
use IPP\Core\Interface\OutputWriter;
use IPP\Core\Interface\InputReader;

class ProgramTag
{
    /** @var InstructionTag[] */
    private $instructions = [];
    /** @var int[] */
    private $labels = [];

    // method adds instruction to array
    public function addInstruction(InstructionTag $instr): void
    {
        $this->instructions[] = $instr;
    }

    // method returns instruction array
    /** @return InstructionTag[] */
    public function getInstructions(): array
    {
        return $this->instructions;
    }

    // method sorts instructions in array
    public function sortInstructions(): void
    {
        usort($this->instructions, array($this, 'sortOrders'));
    }

    // helper method for sorting
    private function sortOrders(InstructionTag $first, InstructionTag $second): int
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
    public function executeProgram(OutputWriter $stdout, OutputWriter $stderr, InputReader $inputReader): void
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
    private function findLabels(): void
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
