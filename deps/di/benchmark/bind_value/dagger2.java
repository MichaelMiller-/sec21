//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
import dagger.*;

@Module
class AppModule {
  @Provides int provideint() { return 42; }
}

@Component(modules = AppModule.class)
interface AppComponent {
  int build();
}

public class dagger2 {
  public static void main(String[] args) {
    Dagger_AppComponent.create().build();
  }
}
