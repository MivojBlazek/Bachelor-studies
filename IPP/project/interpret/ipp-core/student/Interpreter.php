<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

use DOMElement;
use IPP\Core\AbstractInterpreter;
use IPP\Core\Exception\NotImplementedException;
use IPP\Core\Settings;
use IPP\Student\ProgramTag;
use IPP\Student\InstructionTag;
use IPP\Student\ArgumentTag;

class Interpreter extends AbstractInterpreter
{
    public function execute(): int
    {
        // check arguments 
        $set = new Settings;
        $set->processArgs();
        if ($set->isHelp())
        {
            $set->getHelpString();
            exit(0);
        }
        
        // Check \IPP\Core\AbstractInterpreter for predefined I/O objects:
        $dom = $this->source->getDOMDocument();

        // making objects from xml document
        $prog = new ProgramTag();

        $program = $dom->getElementsByTagName('program')->item(0);
        if ($program instanceof DOMElement)
        {
            $language = $program->getAttribute('language');
            if ($language !== 'IPPcode24')
            {
                exit(31); //! invalid attribute language in header
            }

            $instructions = $dom->getElementsByTagName('instruction');
            foreach ($instructions as $instr)
            {
                $opcode = $instr->getAttribute('opcode');
                $order = intval($instr->getAttribute('order'));
                $args = [];
                $instruction = new InstructionTag($opcode, $order, $args);
                $prog->addInstruction($instruction);

                $arg1 = false;
                $arg2 = false;
                foreach ($instr->childNodes as $argName)
                {
                    if ($argName instanceof DOMElement)
                    {
                        if ($argName->nodeName === 'arg1')
                        {
                            $argType = $argName->getAttribute('type');
                            if (empty($argType))
                            {
                                exit(31);
                            }
                            $argValue = $argName->nodeValue;
                            if ($argValue === null)
                            {
                                exit(31);
                            }
                            $arg = new ArgumentTag($argType, trim($argValue));
                            $instruction->addArgument($arg);
                            $arg1 = true;
                        }
                        if ($argName->nodeName === 'arg2' && $arg1)
                        {
                            $argType = $argName->getAttribute('type');
                            if (empty($argType))
                            {
                                exit(31);
                            }
                            $argValue = $argName->nodeValue;
                            if ($argValue === null)
                            {
                                exit(31);
                            }
                            $arg = new ArgumentTag($argType, trim($argValue));
                            $instruction->addArgument($arg);
                            $arg2 = true;
                        }
                        if ($argName->nodeName === 'arg3' && $arg1 && $arg2)
                        {
                            $argType = $argName->getAttribute('type');
                            if (empty($argType))
                            {
                                exit(31);
                            }
                            $argValue = $argName->nodeValue;
                            if ($argValue === null)
                            {
                                exit(31);
                            }
                            $arg = new ArgumentTag($argType, trim($argValue));
                            $instruction->addArgument($arg);
                        }
                    }
                }
            }
        }

        // sort instructions
        try
        {
            $prog->sortInstructions();
        }
        catch (\Exception)
        {
            exit(32);
        }

        // execute program
        $prog->executeProgram($this->stdout, $this->stderr, $this->input);

        exit(0);
    }
}
