import { useEffect, useState } from 'react';
import { Spinner, Table } from "react-bootstrap";
import { useHistory } from 'react-router-dom';
import { DbProject } from './DatabaseInterface';
import axios from 'axios';
import CopyButton from './ui/CopyButton';
import RemoveButton from './ui/RemoveButton';
import OpenButton from './ui/OpenButton';
import Header from "./ui/Header";

type ProjectListItemProps = {
   id: number;
   name: string;
   title: string;
   onDeleteItem: (id: number) => void;
}

function ProjectListItem(props: ProjectListItemProps) {
   const history = useHistory();

   const onOpen = () => {
      history.push("/project/" + props.id);
   }
   const onCopy = () => {
      //! \todo 
   }
   const onDelete = () => {
      props.onDeleteItem(props.id);
   }

   return (
      <tr>
         <td><OpenButton onClick={onOpen} disabled={false} /></td>
         <td>{props.name}</td>
         <td>{props.title}</td>
         <td>TODO</td>
         <td>TODO</td>
         <td><CopyButton onClick={onCopy} disabled={true} /></td>
         <td><RemoveButton onClick={onDelete} disabled={false} /></td>
      </tr>
   )
}

const ProjectList = () => {

   const [result, setResult] = useState<DbProject[]>([])
   const [loading, setLoading] = useState(true)

   const onDelete = (id: number) => {
      //! \todo 
   }

   const getData = async () => {
      await axios.get(process.env.REACT_APP_BACKEND + '/projects')
         .then(response => setResult(response.data))
         .finally(() => setLoading(false))
   }

   useEffect(() => { getData() }, []);

   if (loading)
      return (<Spinner animation="border" role="status"></Spinner>);

   //! \todo error handling

   return (
      <>
         <Header onBack={() => { }} disabledBackButton={true} title="List of Projects" />
         <Table className="projectlist" borderless hover size="sm">
            <thead>
               <tr>
                  <th></th>
                  <th>Name</th>
                  <th>Description</th>
                  <th>Last modifed</th>
                  <th>Created</th>
                  <th></th>
                  <th></th>
               </tr>
            </thead>
            <tbody>
               {result.map(e => (<ProjectListItem key={e.id} id={e.id} name={e.name} title={e.title} onDeleteItem={onDelete} />))}
            </tbody>
         </Table>
      </>
   );
}

export default ProjectList;
