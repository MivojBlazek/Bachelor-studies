<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

class Frame
{
    private $frameStack;
    private $globalFrame;
    private array $localFrame;
    private $tempFrame;

    public function __construct()
    {
        $this->frameStack = [];
        $this->globalFrame = [];
        $this->frameStack[] = &$this->globalFrame;
    }

    // method creates local frame from temporary frame
    public function createLocalFrame()
    {
        if (!isset($this->tempFrame))
        {
            exit(55);
        }
        $this->localFrame = $this->tempFrame;
        $this->frameStack[] = &$this->localFrame;
        $this->tempFrame = null;
    }

    // method removes local frame and makes temporary frame
    public function popLocalFrame()
    {
        if (count($this->frameStack) > 1)
        {
            $this->tempFrame = array_pop($this->frameStack);
        }
        else
        {
            exit(55);
        }
    }

    // method creates temporary frame
    public function createTempFrame()
    {
        $this->tempFrame = [];
    }

    // method adds variable to correct frame
    public function addVariable($frame, Variable $variable)
    {
        if (($this->getVarIfExists($frame, $variable->getName())) !== null)
        {
            exit(52);
        }
        if ($frame === 'GF')
        {
            $this->globalFrame[] = $variable;
        }
        elseif ($frame === 'LF')
        {
            end($this->frameStack);
            $lastFrameInStack = &$this->frameStack[key($this->frameStack)];
            $lastFrameInStack[] = $variable;
        }
        else
        {
            $this->tempFrame[] = $variable;
        }
    }

    // method to check duplicate declaration of variable
    public function getVarIfExists($frame, $variable) : ?Variable
    {
        if ($frame === 'GF')
        {
            $frameToCheck = &$this->globalFrame;
        }
        elseif ($frame === 'LF')
        {
            end($this->frameStack);
            $frameToCheck = &$this->frameStack[key($this->frameStack)];
        }
        else
        {
            $frameToCheck = &$this->tempFrame;
        }

        foreach ($frameToCheck as $vars)
        {
            if ($vars->getName() === $variable)
            {
                return $vars;
            }
        }
        return null;
    }
}
