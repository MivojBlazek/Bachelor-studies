<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

class Frame
{
    /** @var Variable[][] */
    private $frameStack;
    /** @var Variable[] */
    private $globalFrame;
    /** @var Variable[] */
    private $localFrame;
    /** @var Variable[]|null */
    private $tempFrame;

    public function __construct()
    {
        $this->frameStack = [];
        $this->globalFrame = [];
        $this->frameStack[] = &$this->globalFrame;
    }

    // method creates local frame from temporary frame
    public function createLocalFrame(): void
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
    public function popLocalFrame(): void
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
    public function createTempFrame(): void
    {
        $this->tempFrame = [];
    }

    // method adds variable to correct frame
    public function addVariable(string $frame, Variable $variable): void
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
            if (!isset($this->tempFrame))
            {
                exit(55); //! missing temporary frame
            }
            $this->tempFrame[] = $variable;
        }
    }

    // method to check duplicate declaration of variable
    public function getVarIfExists(string $frame, string $variable) : ?Variable
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
            if (!isset($this->tempFrame))
            {
                exit(55); //! missing temporary frame
            }
            $frameToCheck = &$this->tempFrame;
        }

        if (is_iterable($frameToCheck))
        {
            foreach ($frameToCheck as $vars)
            {
                if ($vars->getName() === $variable)
                {
                    return $vars;
                }
            }
        }
        return null;
    }
}
