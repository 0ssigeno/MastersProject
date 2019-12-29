package semanticLib;

public class NameAndPosition {
        public String name;
        public int level;
        public NameAndPosition(String name, int level){
            this.level=level;
            this.name=name;
        }

        @Override
        public String toString() {
            return this.name+this.level;
        }

}
