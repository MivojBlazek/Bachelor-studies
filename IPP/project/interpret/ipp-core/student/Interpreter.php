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

        $tags = $dom->getElementsByTagName('*');
        foreach ($tags as $tag)
        {
            if (!in_array($tag->tagName, ['program', 'instruction', 'arg1', 'arg2', 'arg3']))
            {
                exit(32); //! unknown element
            }
        }

        $program = $dom->getElementsByTagName('program')->item(0);
        if ($program instanceof DOMElement)
        {
            $language = $program->getAttribute('language');
            if ($language !== 'IPPcode24')
            {
                exit(32); //! invalid attribute language in header
            }

            $instructions = $dom->getElementsByTagName('instruction');
            foreach ($instructions as $instr)
            {
                $opcode = $instr->getAttribute('opcode');
                if ($opcode === '')
                {
                    exit(32);
                }
                $order = intval($instr->getAttribute('order'));
                $args = [];
                $instruction = new InstructionTag($opcode, $order, $args);
                $prog->addInstruction($instruction);

                $argTag1 = new ArgumentTag('default', 'default');
                $argTag2 = new ArgumentTag('default', 'default');
                $argTag3 = new ArgumentTag('default', 'default');
                $arg1 = false;
                $arg2 = false;
                $arg3 = false;
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
                            $argTag1 = new ArgumentTag($argType, trim($argValue));
                            $arg1 = true;
                        }
                        if ($argName->nodeName === 'arg2')
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
                            $argTag2 = new ArgumentTag($argType, trim($argValue));
                            $arg2 = true;
                        }
                        if ($argName->nodeName === 'arg3')
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
                            $argTag3 = new ArgumentTag($argType, trim($argValue));
                            $arg3 = true;
                        }
                    }
                }
                if (!(($arg3 && $arg2 && $arg1) || (!$arg3 && $arg2 && $arg1) || (!$arg3 && !$arg2 && $arg1) || (!$arg3 && !$arg2 && !$arg1)))
                {
                    exit(32);
                }
                if ($arg1)
                {
                    $instruction->addArgument($argTag1);
                }
                if ($arg2)
                {
                    $instruction->addArgument($argTag2);
                }
                if ($arg3)
                {
                    $instruction->addArgument($argTag3);
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
