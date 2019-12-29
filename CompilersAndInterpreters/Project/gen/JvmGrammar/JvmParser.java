// Generated from /home/ossigeno/IdeaProjects/ComplexStaticAnalysis/src/jvmGrammar/Jvm.g4 by ANTLR 4.7.2
package jvmGrammar;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class JvmParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.7.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, PUSH=3, POP=4, MOVE=5, ADD=6, ADDI=7, TOP=8, SUB=9, MULT=10, 
		DIV=11, STOREW=12, LOADW=13, LOADI=14, BRANCH=15, BRANCHEQ=16, BRANCHNOTEQ=17, 
		BRANCHLESS=18, BRANCHLESSEQ=19, BRANCHGREATER=20, BRANCHGREATEREQ=21, 
		JAL=22, JR=23, PRINT=24, HALT=25, COL=26, REGISTER=27, LABEL=28, NUMBER=29, 
		WHITESP=30, ERR=31;
	public static final int
		RULE_assembly = 0, RULE_instruction = 1;
	private static String[] makeRuleNames() {
		return new String[] {
			"assembly", "instruction"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'('", "')'", "'push'", "'pop'", "'mv'", "'add'", "'addi'", "'top'", 
			"'sub'", "'mult'", "'div'", "'sw'", "'lw'", "'li'", "'b'", "'beq'", "'bne'", 
			"'blt'", "'ble'", "'bge'", "'bgt'", "'jal'", "'jr'", "'print'", "'halt'", 
			"':'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, "PUSH", "POP", "MOVE", "ADD", "ADDI", "TOP", "SUB", 
			"MULT", "DIV", "STOREW", "LOADW", "LOADI", "BRANCH", "BRANCHEQ", "BRANCHNOTEQ", 
			"BRANCHLESS", "BRANCHLESSEQ", "BRANCHGREATER", "BRANCHGREATEREQ", "JAL", 
			"JR", "PRINT", "HALT", "COL", "REGISTER", "LABEL", "NUMBER", "WHITESP", 
			"ERR"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "Jvm.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public JvmParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class AssemblyContext extends ParserRuleContext {
		public List<InstructionContext> instruction() {
			return getRuleContexts(InstructionContext.class);
		}
		public InstructionContext instruction(int i) {
			return getRuleContext(InstructionContext.class,i);
		}
		public AssemblyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assembly; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof JvmListener ) ((JvmListener)listener).enterAssembly(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof JvmListener ) ((JvmListener)listener).exitAssembly(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof JvmVisitor ) return ((JvmVisitor<? extends T>)visitor).visitAssembly(this);
			else return visitor.visitChildren(this);
		}
	}

	public final AssemblyContext assembly() throws RecognitionException {
		AssemblyContext _localctx = new AssemblyContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_assembly);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(7);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << PUSH) | (1L << POP) | (1L << MOVE) | (1L << ADD) | (1L << ADDI) | (1L << TOP) | (1L << SUB) | (1L << MULT) | (1L << DIV) | (1L << STOREW) | (1L << LOADW) | (1L << LOADI) | (1L << BRANCH) | (1L << BRANCHEQ) | (1L << BRANCHNOTEQ) | (1L << BRANCHLESS) | (1L << BRANCHLESSEQ) | (1L << BRANCHGREATER) | (1L << BRANCHGREATEREQ) | (1L << JAL) | (1L << JR) | (1L << PRINT) | (1L << HALT) | (1L << LABEL))) != 0)) {
				{
				{
				setState(4);
				instruction();
				}
				}
				setState(9);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class InstructionContext extends ParserRuleContext {
		public Token r1;
		public Token r2;
		public Token r3;
		public Token n;
		public Token l;
		public TerminalNode PUSH() { return getToken(JvmParser.PUSH, 0); }
		public TerminalNode POP() { return getToken(JvmParser.POP, 0); }
		public TerminalNode ADD() { return getToken(JvmParser.ADD, 0); }
		public TerminalNode ADDI() { return getToken(JvmParser.ADDI, 0); }
		public TerminalNode SUB() { return getToken(JvmParser.SUB, 0); }
		public TerminalNode MULT() { return getToken(JvmParser.MULT, 0); }
		public TerminalNode DIV() { return getToken(JvmParser.DIV, 0); }
		public TerminalNode STOREW() { return getToken(JvmParser.STOREW, 0); }
		public TerminalNode LOADW() { return getToken(JvmParser.LOADW, 0); }
		public TerminalNode LOADI() { return getToken(JvmParser.LOADI, 0); }
		public TerminalNode COL() { return getToken(JvmParser.COL, 0); }
		public TerminalNode BRANCH() { return getToken(JvmParser.BRANCH, 0); }
		public TerminalNode BRANCHNOTEQ() { return getToken(JvmParser.BRANCHNOTEQ, 0); }
		public TerminalNode BRANCHEQ() { return getToken(JvmParser.BRANCHEQ, 0); }
		public TerminalNode BRANCHLESS() { return getToken(JvmParser.BRANCHLESS, 0); }
		public TerminalNode BRANCHLESSEQ() { return getToken(JvmParser.BRANCHLESSEQ, 0); }
		public TerminalNode BRANCHGREATER() { return getToken(JvmParser.BRANCHGREATER, 0); }
		public TerminalNode BRANCHGREATEREQ() { return getToken(JvmParser.BRANCHGREATEREQ, 0); }
		public TerminalNode MOVE() { return getToken(JvmParser.MOVE, 0); }
		public TerminalNode JR() { return getToken(JvmParser.JR, 0); }
		public TerminalNode JAL() { return getToken(JvmParser.JAL, 0); }
		public TerminalNode PRINT() { return getToken(JvmParser.PRINT, 0); }
		public TerminalNode TOP() { return getToken(JvmParser.TOP, 0); }
		public TerminalNode HALT() { return getToken(JvmParser.HALT, 0); }
		public List<TerminalNode> REGISTER() { return getTokens(JvmParser.REGISTER); }
		public TerminalNode REGISTER(int i) {
			return getToken(JvmParser.REGISTER, i);
		}
		public TerminalNode NUMBER() { return getToken(JvmParser.NUMBER, 0); }
		public TerminalNode LABEL() { return getToken(JvmParser.LABEL, 0); }
		public InstructionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_instruction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof JvmListener ) ((JvmListener)listener).enterInstruction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof JvmListener ) ((JvmListener)listener).exitInstruction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof JvmVisitor ) return ((JvmVisitor<? extends T>)visitor).visitInstruction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final InstructionContext instruction() throws RecognitionException {
		InstructionContext _localctx = new InstructionContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_instruction);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(87);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case PUSH:
				{
				setState(10);
				match(PUSH);
				setState(11);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				}
				break;
			case POP:
				{
				setState(12);
				match(POP);
				}
				break;
			case ADD:
				{
				setState(13);
				match(ADD);
				setState(14);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(15);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(16);
				((InstructionContext)_localctx).r3 = match(REGISTER);
				}
				break;
			case ADDI:
				{
				setState(17);
				match(ADDI);
				setState(18);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(19);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(20);
				((InstructionContext)_localctx).n = match(NUMBER);
				}
				break;
			case SUB:
				{
				setState(21);
				match(SUB);
				setState(22);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(23);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(24);
				((InstructionContext)_localctx).r3 = match(REGISTER);
				}
				break;
			case MULT:
				{
				setState(25);
				match(MULT);
				setState(26);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(27);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(28);
				((InstructionContext)_localctx).r3 = match(REGISTER);
				}
				break;
			case DIV:
				{
				setState(29);
				match(DIV);
				setState(30);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(31);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(32);
				((InstructionContext)_localctx).r3 = match(REGISTER);
				}
				break;
			case STOREW:
				{
				setState(33);
				match(STOREW);
				setState(34);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(35);
				((InstructionContext)_localctx).n = match(NUMBER);
				setState(36);
				match(T__0);
				setState(37);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(38);
				match(T__1);
				}
				break;
			case LOADW:
				{
				setState(39);
				match(LOADW);
				setState(40);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(41);
				((InstructionContext)_localctx).n = match(NUMBER);
				setState(42);
				match(T__0);
				setState(43);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(44);
				match(T__1);
				}
				break;
			case LOADI:
				{
				setState(45);
				match(LOADI);
				setState(46);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(47);
				((InstructionContext)_localctx).n = match(NUMBER);
				}
				break;
			case LABEL:
				{
				setState(48);
				((InstructionContext)_localctx).l = match(LABEL);
				setState(49);
				match(COL);
				}
				break;
			case BRANCH:
				{
				setState(50);
				match(BRANCH);
				setState(51);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case BRANCHNOTEQ:
				{
				setState(52);
				match(BRANCHNOTEQ);
				setState(53);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(54);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(55);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case BRANCHEQ:
				{
				setState(56);
				match(BRANCHEQ);
				setState(57);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(58);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(59);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case BRANCHLESS:
				{
				setState(60);
				match(BRANCHLESS);
				setState(61);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(62);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(63);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case BRANCHLESSEQ:
				{
				setState(64);
				match(BRANCHLESSEQ);
				setState(65);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(66);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(67);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case BRANCHGREATER:
				{
				setState(68);
				match(BRANCHGREATER);
				setState(69);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(70);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(71);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case BRANCHGREATEREQ:
				{
				setState(72);
				match(BRANCHGREATEREQ);
				setState(73);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(74);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				setState(75);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case MOVE:
				{
				setState(76);
				match(MOVE);
				setState(77);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				setState(78);
				((InstructionContext)_localctx).r2 = match(REGISTER);
				}
				break;
			case JR:
				{
				setState(79);
				match(JR);
				setState(80);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				}
				break;
			case JAL:
				{
				setState(81);
				match(JAL);
				setState(82);
				((InstructionContext)_localctx).l = match(LABEL);
				}
				break;
			case PRINT:
				{
				setState(83);
				match(PRINT);
				}
				break;
			case TOP:
				{
				setState(84);
				match(TOP);
				setState(85);
				((InstructionContext)_localctx).r1 = match(REGISTER);
				}
				break;
			case HALT:
				{
				setState(86);
				match(HALT);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3!\\\4\2\t\2\4\3\t"+
		"\3\3\2\7\2\b\n\2\f\2\16\2\13\13\2\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3"+
		"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3"+
		"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3"+
		"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3"+
		"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\5\3Z\n"+
		"\3\3\3\2\2\4\2\4\2\2\2q\2\t\3\2\2\2\4Y\3\2\2\2\6\b\5\4\3\2\7\6\3\2\2\2"+
		"\b\13\3\2\2\2\t\7\3\2\2\2\t\n\3\2\2\2\n\3\3\2\2\2\13\t\3\2\2\2\f\r\7\5"+
		"\2\2\rZ\7\35\2\2\16Z\7\6\2\2\17\20\7\b\2\2\20\21\7\35\2\2\21\22\7\35\2"+
		"\2\22Z\7\35\2\2\23\24\7\t\2\2\24\25\7\35\2\2\25\26\7\35\2\2\26Z\7\37\2"+
		"\2\27\30\7\13\2\2\30\31\7\35\2\2\31\32\7\35\2\2\32Z\7\35\2\2\33\34\7\f"+
		"\2\2\34\35\7\35\2\2\35\36\7\35\2\2\36Z\7\35\2\2\37 \7\r\2\2 !\7\35\2\2"+
		"!\"\7\35\2\2\"Z\7\35\2\2#$\7\16\2\2$%\7\35\2\2%&\7\37\2\2&\'\7\3\2\2\'"+
		"(\7\35\2\2(Z\7\4\2\2)*\7\17\2\2*+\7\35\2\2+,\7\37\2\2,-\7\3\2\2-.\7\35"+
		"\2\2.Z\7\4\2\2/\60\7\20\2\2\60\61\7\35\2\2\61Z\7\37\2\2\62\63\7\36\2\2"+
		"\63Z\7\34\2\2\64\65\7\21\2\2\65Z\7\36\2\2\66\67\7\23\2\2\678\7\35\2\2"+
		"89\7\35\2\29Z\7\36\2\2:;\7\22\2\2;<\7\35\2\2<=\7\35\2\2=Z\7\36\2\2>?\7"+
		"\24\2\2?@\7\35\2\2@A\7\35\2\2AZ\7\36\2\2BC\7\25\2\2CD\7\35\2\2DE\7\35"+
		"\2\2EZ\7\36\2\2FG\7\26\2\2GH\7\35\2\2HI\7\35\2\2IZ\7\36\2\2JK\7\27\2\2"+
		"KL\7\35\2\2LM\7\35\2\2MZ\7\36\2\2NO\7\7\2\2OP\7\35\2\2PZ\7\35\2\2QR\7"+
		"\31\2\2RZ\7\35\2\2ST\7\30\2\2TZ\7\36\2\2UZ\7\32\2\2VW\7\n\2\2WZ\7\35\2"+
		"\2XZ\7\33\2\2Y\f\3\2\2\2Y\16\3\2\2\2Y\17\3\2\2\2Y\23\3\2\2\2Y\27\3\2\2"+
		"\2Y\33\3\2\2\2Y\37\3\2\2\2Y#\3\2\2\2Y)\3\2\2\2Y/\3\2\2\2Y\62\3\2\2\2Y"+
		"\64\3\2\2\2Y\66\3\2\2\2Y:\3\2\2\2Y>\3\2\2\2YB\3\2\2\2YF\3\2\2\2YJ\3\2"+
		"\2\2YN\3\2\2\2YQ\3\2\2\2YS\3\2\2\2YU\3\2\2\2YV\3\2\2\2YX\3\2\2\2Z\5\3"+
		"\2\2\2\4\tY";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}