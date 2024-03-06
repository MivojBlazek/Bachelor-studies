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
        //$prog = $this->processXML($dom);
        $prog = new ProgramTag();

        $program = $dom->getElementsByTagName('program')->item(0);
        if ($program instanceof DOMElement)
        {
            $language = $program->getAttribute('language');
            //TODO check if language is correct
            // $prog = new ProgramTag();

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
                            $arg = new ArgumentTag($argName->getAttribute('type'), trim($argName->nodeValue));
                            $instruction->addArgument($arg);
                            $arg1 = true;
                        }
                        if ($argName->nodeName === 'arg2' && $arg1)
                        {
                            $arg = new ArgumentTag($argName->getAttribute('type'), trim($argName->nodeValue));
                            $instruction->addArgument($arg);
                            $arg2 = true;
                        }
                        if ($argName->nodeName === 'arg3' && $arg1 && $arg2)
                        {
                            $arg = new ArgumentTag($argName->getAttribute('type'), trim($argName->nodeValue));
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

    // method returns program after it process XML source and create program, instructions and arguments objects
    /** @param DOMDocument $dom */
    //protected function processXML(DOMDocument $dom): ProgramTag
    //{
   //     return $prog;
    //}
}
