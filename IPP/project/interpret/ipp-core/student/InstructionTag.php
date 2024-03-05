<?php
/**
 * IPP - PHP Project Student
 * @author Michal Blažek
 * 
 */

namespace IPP\Student;

use IPP\Student\Variable;
use IPP\Student\Frame;
use IPP\Student\Stack;

class InstructionTag
{
    private $opcode;
    protected $order;
    private $args = [];

    public function __construct($opcode, $order, $args)
    {
        $this->opcode = strtoupper($opcode);
        $this->order = $order;
        $this->args = $args;
    }

    // method adds argument to instruction
    public function addArgument(ArgumentTag $arg)
    {
        $this->args[] = $arg;
    }

    // method returns operation code of instruction
    public function getOpcode()
    {
        return $this->opcode;
    }

    // method returns order of instruction
    public function getOrder()
    {
        return $this->order;
    }

    // method returns arguments of instruction
    public function getArguments()
    {
        return $this->args;
    }

    // main method to execute instruction
    public function executeInstr($instrIndex, $labels, $frames, $stack, $functionStack, $stdout, $stderr, $inputReader)
    {
        // default setting arg variables
        $arg1Type = 'nil';
        $arg1Value = 'nil';
        $arg2Type = 'nil';
        $arg2Value = 'nil';
        $arg3Type = 'nil';
        $arg3Value = 'nil';

        // get arguments of instruction
        if (isset($this->args[0]))
        {
            $arg1Type = $this->args[0]->getType();
            $arg1Value = $this->args[0]->getValue();
        }
        if (isset($this->args[1]))
        {
            $arg2Type = $this->args[1]->getType();
            $arg2Value = $this->args[1]->getValue();
        }
        if (isset($this->args[2]))
        {
            $arg3Type = $this->args[2]->getType();
            $arg3Value = $this->args[2]->getValue();
        }

        // select command of instruction
        switch ($this->opcode)
        {
            case 'MOVE':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54); //? mby check
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    $variable->setValue($variable2->getValue());
                }
                else
                {
                    $variable->setValue($arg2Value);
                }
                break;
            case 'CREATEFRAME':
                $frames->createTempFrame();
                break;
            case 'PUSHFRAME':
                $frames->createLocalFrame();
                break;
            case 'POPFRAME':
                $frames->popLocalFrame();
                break;
            case 'DEFVAR':
                list($frame, $variable) = explode('@', $arg1Value);
                $var = new Variable($variable);
                $frames->addVariable($frame, $var);
                break;
            case 'CALL':
                if ($arg1Type !== 'label')
                {
                    exit(53);
                }
                if (!isset($labels[$arg1Value]))
                {
                    exit(52);
                }
                $functionStack->stackPush('#CALL#' . $instrIndex);
                return $arg1Value;
            case 'RETURN':
                return $functionStack->stackPop();
            case 'PUSHS':
                if ($arg1Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg1Value);
                    $variable = $frames->getVarIfExists($frame, $var);
                    if ($variable === null)
                    {
                        exit(54);
                    }
                    $stack->stackPush($variable->getValue());
                }
                else
                {
                    $stack->stackPush($arg1Value);
                }
                break;
            case 'POPS':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                $variable->setValue($stack->stackPop());
                break;
            case 'ADD':
            case 'SUB':
            case 'MUL':
            case 'IDIV':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }

                if ($arg2Type === 'var' && $arg3Value !== '')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'int')
                    {
                        exit(53);
                    }
                    $variable->setValue($variable2->getValue());
                }
                elseif ($arg2Type === 'int')
                {
                    $variable->setValue($arg2Value);
                }
                else
                {
                    exit(53);
                }
                
                $result = '0';
                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    if ($variable3->getType() !== 'int')
                    {
                        exit(53);
                    }
                    switch ($this->opcode)
                    {
                        case 'ADD':
                            $result = floor($variable->getValue() + $variable3->getValue());
                            break;
                        case 'SUB':
                            $result = floor($variable->getValue() - $variable3->getValue());
                            break;
                        case 'MUL':
                            $result = floor($variable->getValue() * $variable3->getValue());
                            break;
                        case 'IDIV':
                            if ($variable3->getValue() === '0')
                            {
                                exit(57); //! division by zero
                            }
                            $result = floor($variable->getValue() / $variable3->getValue());
                            break;
                    }
                    $variable->setValue($result);
                }
                elseif ($arg3Type === 'int' && $arg3Value !== '')
                {
                    switch ($this->opcode)
                    {
                        case 'ADD':
                            $result = floor($variable->getValue() + $arg3Value);
                            break;
                        case 'SUB':
                            $result = floor($variable->getValue() - $arg3Value);
                            break;
                        case 'MUL':
                            $result = floor($variable->getValue() * $arg3Value);
                            break;
                        case 'IDIV':
                            if ($arg3Value === '0')
                            {
                                exit(57); //! division by zero
                            }
                            $result = floor($variable->getValue() / $arg3Value);
                            break;
                    }
                    $variable->setValue($result);
                }
                else
                {
                    exit(53);
                }
                break;
            case 'LT':
            case 'GT':
            case 'EQ':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    $first = $variable2->getValue();
                    $firstType = $variable2->getType();
                }
                else
                {
                    $first = $arg2Value;
                    $firstType = $arg2Type;
                }
                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    $second = $variable3->getValue();
                    $secondType = $variable3->getType();
                }
                else
                {
                    $second = $arg3Value;
                    $secondType = $arg3Type;
                }

                if ($firstType === 'nil' || $secondType === 'nil')
                {
                    if ($this->opcode === 'EQ')
                    {
                        if ($first === $second)
                        {
                            $variable->setValue('true');
                        }
                        else
                        {
                            $variable->setValue('false');
                        }
                    }
                    else
                    {
                        exit(53);
                    }
                }
                else
                {
                    if ($firstType !== $secondType)
                    {
                        exit(53);
                    }
                    if ($this->opcode === 'GT')
                    {
                        $result = ($first > $second);
                    }
                    elseif ($this->opcode === 'LT')
                    {
                        $result = ($first < $second);
                    }
                    else
                    {
                        $result = ($first == $second);
                    }
                    $variable->setValue($result ? 'true' : 'false');
                }
                break;
            case 'AND':
            case 'OR':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'bool')
                    {
                        exit(53);
                    }
                    $bool1 = $variable2->getValue();
                }
                elseif ($arg2Type === 'bool')
                {
                    $bool1 = $arg2Value;
                }
                else
                {
                    exit(53);
                }

                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    if ($variable3->getType() !== 'bool')
                    {
                        exit(53);
                    }
                    $bool2 = $variable3->getValue();
                }
                elseif ($arg3Type === 'bool')
                {
                    $bool2 = $arg3Value;
                }
                else
                {
                    exit(53);
                }

                $b1 = false;
                $b2 = false;
                if ($bool1 === 'true')
                {
                    $b1 = true;
                }
                if ($bool2 === 'true')
                {
                    $b2 = true;
                }

                if ($this->opcode === 'AND')
                {
                    if ($b1 && $b2)
                    {
                        $variable->setValue('true');
                    }
                    else
                    {
                        $variable->setValue('false');
                    }
                }
                else
                {
                    if ($b1 || $b2)
                    {
                        $variable->setValue('true');
                    }
                    else
                    {
                        $variable->setValue('false');
                    }
                }
                break;
            case 'NOT':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'bool')
                    {
                        exit(53);
                    }
                    $value = $variable2->getValue();
                }
                elseif ($arg2Type === 'bool')
                {
                    $value = $arg2Value;
                }
                else
                {
                    exit(53);
                }
                if ($value === 'true')
                {
                    $variable->setValue('false');
                }
                else
                {
                    $variable->setValue('true');
                }
                break;
            case 'INT2CHAR':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'int')
                    {
                        exit(53);
                    }
                    $value = $variable2->getValue();
                }
                elseif ($arg2Type === 'int')
                {
                    $value = $arg2Value;
                }
                else
                {
                    exit(53);
                }

                if (intval($value) < 0 || intval($value) > 255)
                {
                    exit(58);
                }
                $variable->setValue(chr(intval($value)));
                break;
            case 'STRI2INT':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'string')
                    {
                        exit(53);
                    }
                    $string = $variable2->getValue();
                }
                elseif ($arg2Type === 'string')
                {
                    $string = $arg2Value;
                }
                else
                {
                    exit(53);
                }

                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    if ($variable3->getType() !== 'int')
                    {
                        exit(53);
                    }
                    $position = $variable3->getValue();
                }
                elseif ($arg3Type === 'int')
                {
                    $position = $arg3Value;
                }
                else
                {
                    exit(53);
                }

                if (intval($position) < 0 || intval($position) >= strlen($string))
                {
                    exit(58);
                }
                $variable->setValue(ord($string[intval($position)]));
                break;
            case 'READ':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'type')
                {
                    if ($arg2Value !== 'int' && $arg2Value !== 'string' && $arg2Value !== 'bool')
                    {
                        exit(57);
                    }
                }
                else
                {
                    exit(53);
                }
                switch ($arg2Value)
                {
                    case 'int':
                        $read = $inputReader->readInt();
                        if ($read === null)
                        {
                            $read = 'nil';
                        }
                        break;
                    case 'bool':
                        $read = $inputReader->readBool();
                        if ($read === null) //? works with == but '0' is 'nil', 'false' is 'nil'
                        {
                            $read = 'nil';
                        }
                        else
                        {
                            $read = $read ? 'true' : 'false';
                        }
                        break;
                    case 'string':
                        $read = $inputReader->readString();
                        if ($read === null)
                        {
                            $read = 'nil';
                        }
                        break;
                    default:
                        exit(53);
                }
                $variable->setValue($read);
                break;
            case 'WRITE':
                if ($arg1Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg1Value);
                    $variable = $frames->getVarIfExists($frame, $var);
                    if ($variable === null)
                    {
                        exit(54);
                    }
                    $value = $variable->getValueNil();
                    if ($value === 'nil')
                    {
                        $stdout->writeString('');
                    }
                    else
                    {
                        $valueParts = explode('\\', $value);
                        $firstItem = true;
                        foreach ($valueParts as &$val)
                        {
                            if ($firstItem)
                            {
                                $firstItem = false;
                                continue;
                            }
                            if (strlen($val) >= 3)
                            {
                                $decimal = substr($val, 0, 3);
                                if (!ctype_digit($decimal))
                                {
                                    continue;
                                }
                                $decimal = intval($decimal);
                                $octal = decoct($decimal);
                                $octal = str_pad($octal, 3, '0', STR_PAD_LEFT);
                                $val = $octal . substr($val, 3);
                            }
                            else
                            {
                                exit(57); //! invalid escape sequence
                            }
                        }
                        $stdout->writeString(stripcslashes(implode('\\', $valueParts)));
                    }
                }
                else
                {
                    $valueParts = explode('\\', $arg1Value);
                    $firstItem = true;
                    foreach ($valueParts as &$val)
                    {
                        if ($firstItem)
                        {
                            $firstItem = false;
                            continue;
                        }
                        if (strlen($val) >= 3)
                        {
                            $decimal = substr($val, 0, 3);
                            if (!ctype_digit($decimal))
                            {
                                continue;
                            }
                            $decimal = intval($decimal);
                            $octal = decoct($decimal);
                            $octal = str_pad($octal, 3, '0', STR_PAD_LEFT);
                            $val = $octal . substr($val, 3);
                        }
                        else
                        {
                            exit(57); //! invalid escape sequence
                        }
                    }
                    $stdout->writeString(stripcslashes(implode('\\', $valueParts)));
                }
                break;
            case 'CONCAT':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'string')
                    {
                        exit(53);
                    }
                    $string1 = $variable2->getValue();
                }
                elseif ($arg2Type === 'string')
                {
                    $string1 = $arg2Value;
                }
                else
                {
                    exit(53);
                }

                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    if ($variable3->getType() !== 'string')
                    {
                        exit(53);
                    }
                    $string2 = $variable3->getValue();
                }
                elseif ($arg3Type === 'string')
                {
                    $string2 = $arg3Value;
                }
                else
                {
                    exit(53);
                }
                $variable->setValue($string1 . $string2);
                break;
            case 'STRLEN':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'string')
                    {
                        exit(53);
                    }
                    $variable->setValue(strlen($variable2->getValue()));
                }
                elseif ($arg2Type === 'string')
                {
                    $variable->setValue(strlen($arg2Value));
                }
                else
                {
                    exit(53);
                }
                break;
            case 'GETCHAR':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'string')
                    {
                        exit(53);
                    }
                    $string = $variable2->getValue();
                }
                elseif ($arg2Type === 'string')
                {
                    $string = $arg2Value;
                }
                else
                {
                    exit(53);
                }

                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    if ($variable3->getType() !== 'int')
                    {
                        exit(53);
                    }
                    $position = $variable3->getValue();
                }
                elseif ($arg3Type === 'int')
                {
                    $position = $arg3Value;
                }
                else
                {
                    exit(53);
                }

                if (intval($position) < 0 || intval($position) >= strlen($string))
                {
                    exit(58);
                }
                $variable->setValue($string[intval($position)]);
                break;
            case 'SETCHAR':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($variable->getType() !== 'string')
                {
                    exit(53);
                }
                $string = $variable->getValue();

                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    if ($variable2->getType() !== 'int')
                    {
                        exit(53);
                    }
                    $position = $variable2->getValue();
                }
                elseif ($arg2Type === 'int')
                {
                    $position = $arg2Value;
                }
                else
                {
                    exit(53);
                }
                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    if ($variable3->getType() !== 'string')
                    {
                        exit(53);
                    }
                    $character = $variable3->getValue();
                }
                elseif ($arg3Type === 'string')
                {
                    $character = $arg3Value;
                }
                else
                {
                    exit(53);
                }

                if ((intval($position) < 0 || intval($position) >= strlen($string)) || ($character === ''))
                {
                    exit(58);
                }
                $character = $character[0];
                $string[intval($position)] = $character;
                $variable->setValue($string);
                break;
            case 'TYPE':
                list($frame, $var) = explode('@', $arg1Value);
                $variable = $frames->getVarIfExists($frame, $var);
                if ($variable === null)
                {
                    exit(54);
                }
                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    $variable->setValue($variable2->getType());
                }
                else
                {
                    $variable->setValue($arg2Type);
                }
                break;
            case 'LABEL':
                // nothing to do because ProgramTag made it
                break;
            case 'JUMP':
                if ($arg1Type !== 'label')
                {
                    exit(53);
                }
                if (!isset($labels[$arg1Value]))
                {
                    exit(52);
                }
                return $arg1Value;
            case 'JUMPIFEQ':
            case 'JUMPIFNEQ':
                if ($arg1Type !== 'label')
                {
                    exit(53);
                }
                if (!isset($labels[$arg1Value]))
                {
                    exit(52);
                }

                if ($arg2Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg2Value);
                    $variable2 = $frames->getVarIfExists($frame, $var);
                    if ($variable2 === null)
                    {
                        exit(54);
                    }
                    $first = $variable2->getValue();
                    $firstType = $variable2->getType();
                }
                else
                {
                    $first = $arg2Value;
                    $firstType = $arg2Type;
                }
                if ($arg3Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg3Value);
                    $variable3 = $frames->getVarIfExists($frame, $var);
                    if ($variable3 === null)
                    {
                        exit(54);
                    }
                    $second = $variable3->getValue();
                    $secondType = $variable3->getType();
                }
                else
                {
                    $second = $arg3Value;
                    $secondType = $arg3Type;
                }

                if ($firstType !== $secondType)
                {
                    exit(53);
                }
                if ($this->opcode === 'JUMPIFEQ')
                {
                    if ($first == $second)
                    {
                        return $arg1Value;
                    }
                }
                else
                {
                    if ($first != $second)
                    {
                        return $arg1Value;
                    }
                }
                break;
            case 'EXIT':
                if ($arg1Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg1Value);
                    $variable = $frames->getVarIfExists($frame, $var);
                    if ($variable === null)
                    {
                        exit(54);
                    }
                    if ($variable->getType() !== 'int')
                    {
                        exit(53);
                    }
                    $value = $variable->getValue();
                }
                elseif ($arg1Type === 'int')
                {
                    $value = $arg1Value;
                }
                else
                {
                    exit(53);
                }

                if ($value < 0 || $value > 9)
                {
                    exit(57);
                }
                exit(intval($value));
            case 'DPRINT':
                if ($arg1Type === 'var')
                {
                    list($frame, $var) = explode('@', $arg1Value);
                    $variable = $frames->getVarIfExists($frame, $var);
                    if ($variable === null)
                    {
                        exit(54);
                    }
                    $stderr->writeString($variable->getValue());
                }
                else
                {
                    $stderr->writeString($arg1Value);
                }
                break;
            case 'BREAK': //TODO
                $stderr->writeString('Info about current position in code');
                break;
            default:
                // should not occur if opcode is correct from parse.py
                break;
        }
    }
}
