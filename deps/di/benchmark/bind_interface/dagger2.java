//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
import dagger.*;
import javax.inject.Inject;

interface I {
  public void dummy();
};

class Impl implements I {
  @Inject Impl() { }
  @Override public void dummy() { }
}

@Module
class AppModule {
  @Provides I provideI(Impl impl) { return impl; }
}

@Component(modules = AppModule.class)
interface AppComponent {
  I build();
}

public class dagger2 {
  public static void main(String[] args) {
    I object = Dagger_AppComponent.create().build();
   }
}
