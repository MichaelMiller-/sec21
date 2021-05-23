import { Container, Jumbotron } from 'react-bootstrap';
import './App.css';
import { Redirect, Route, Switch } from "react-router-dom";
import { ProjectList } from './ProjectList';
import Project, { RouterComponentProjectProps } from './Project';
import { MaterialList } from './MaterialList';
import { CrossSectionList, RouterComponentCrossSectionListProps } from './CrossSectionList';
import Structure, { RouterComponentStructureProps } from './Structure';
import { Material, RouterComponentMaterialProps } from './Material';
import { CrossSection, RouterComponentCrossSectionProps } from './CrossSection';
import { RouterComponentStructuralPointProps, StructuralPoint } from './StructuralPoint';


function App() {
  return (
    <Container className="p-3">
        <Jumbotron>
          <h1 className="header">
            sec21 frontend
          </h1>
        </Jumbotron>
        <Switch>
          <Route exact path="/404">not found</Route>
          <Route exact path="/"><ProjectList /></Route>
          <Route
            exact
            path="/project/:projectId"
            render={({ match }: RouterComponentProjectProps) => (<Project projectId={match.params.projectId} />)}
          />
          <Route
            exact
            path="/structure/:projectId"
            render={({ match }: RouterComponentStructureProps) => (<Structure projectId={match.params.projectId} />)}
          />
          <Route
            exact
            path="/materials/:projectId"
            render={({ match }: RouterComponentProjectProps) => (<MaterialList projectId={match.params.projectId} />)}
          />
          <Route
            exact
            path="/crosssections/:projectId"
            render={({ match }: RouterComponentCrossSectionListProps) => (<CrossSectionList projectId={match.params.projectId} />)}
          />
          <Route
            exact
            path="/crosssection/:projectId"
            render={({ match }: RouterComponentCrossSectionProps) => (<CrossSection projectId={match.params.projectId} />)}
          />
          <Route
            exact
            path="/material/:projectId"
            render={({ match }: RouterComponentMaterialProps) => (<Material projectId={match.params.projectId} />)}
          />
          <Route
            exact
            path="/structuralpoint/:projectId"
            render={({ match }: RouterComponentStructuralPointProps) => (<StructuralPoint projectId={match.params.projectId} />)}
          />
          <Redirect to="/404" /> 
        </Switch>
    </Container>
  );
}
export default App;
