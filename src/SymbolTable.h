// $Id$
#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <QString>
#include <QList>
#include <QMultiMap>
#include <QMultiHash>

struct MemoryLayout;

class SymbolTable;

class Symbol
{
public:
	Symbol( const QString& str, int addr, int val = 0xFFFF );
	
	friend class SymbolTable;

	enum SymbolStatus { ACTIVE, HIDDEN, LOST };
	enum SymbolType { JUMPLABEL, VARIABLELABEL, VALUE };
	enum Register { REG_A = 1, REG_B = 2, REG_C = 4, REG_D = 8, REG_E = 16,
	                REG_H = 32, REG_L = 64, REG_BC = 128, REG_DE = 256,
	                REG_HL = 512, REG_IX = 1024, REG_IY = 2048, REG_IXL = 4096,
	                REG_IXH = 8192, REG_IYL = 16384, REG_IYH = 32768,
	                REG_ALL8 = 1+2+4+8+16+32+64+4096+8192+16384+32768,
	                REG_ALL16 = 128+256+512+1024+2048,
	                REG_ALL = 65535 };
	
	const QString& text() const;
	void setText( const QString& str );
	int value() const;
	void setValue( int addr );
	int validSlots() const;
	void setValidSlots( int val );
	int validRegisters() const;
	void setValidRegisters( int regs );
	const QString *source() const;
	void setSource( QString* name );
	SymbolStatus status() const;
	void setStatus( SymbolStatus s );
	SymbolType type() const;
	void setType( SymbolType t );
	
	bool isSlotValid( MemoryLayout *ml = 0 );

private:
	SymbolTable *table;

	QString symText;
	int symValue;
	int symSlots;
	QList<unsigned char> symSegments;
	int symRegisters;
	QString *symSource;
	SymbolStatus symStatus;
	SymbolType symType;
};


class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	void add( Symbol *symbol );
	void removeAt( int index );

	/* Symbol access functions */
	Symbol *findFirstAddressSymbol( int addr, MemoryLayout *ml = 0 );
	Symbol *getCurrentAddressSymbol();
	Symbol *findNextAddressSymbol( MemoryLayout *ml = 0 );
	Symbol *getValueSymbol( int val, Symbol::Register reg, MemoryLayout *ml = 0 );
	Symbol *getAddressSymbol( int val, MemoryLayout *ml = 0 );

	void symbolTypeChanged( Symbol *symbol );
	void symbolValueChanged( Symbol *symbol );

	int symbolFilesSize() const;
	const QString& symbolFile( int index ) const;

	bool readTNIASM0File( const QString& filename );
	bool readASMSXFile( const QString& filename );
	void reloadFiles();
	void unloadFile( const QString& file, bool keepSymbols = false );

private:
	QList<Symbol*> symbols;
	QMultiMap<int, Symbol*> addressSymbols;
	QMultiHash<int, Symbol*> valueSymbols;
	QMultiMap<int, Symbol*>::iterator currentAddress;
	QList<QString*> symbolFiles;

	void mapSymbol( Symbol *symbol );
	void unmapSymbol( Symbol *symbol );
};


#endif // _SYMBOLTABLE_H
